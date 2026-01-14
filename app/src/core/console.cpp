#include "console.h"
#include <stdlib.h>
#include "string.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>
#include "event_machine.hpp"

LOG_MODULE_REGISTER(CONSOLE);

const struct device *uart0 = DEVICE_DT_GET(DT_NODELABEL(uart0));
K_MSGQ_DEFINE(uart_msgq, 20, 10, 4);

#define DATA_COUNT 7

void serial_cb(const struct device *dev, void *user_data) {
    
    static char rx_buf[64];
    static int rx_buf_pos = 0;
	uint8_t c;

	if (!uart_irq_update(uart0)) {
		return;
	}

	if (!uart_irq_rx_ready(uart0)) {
		return;
	}

	while (uart_fifo_read(uart0, &c, 1) == 1) {
		if ((c == '\n' || c == '\r') && rx_buf_pos > 0) {

			rx_buf[rx_buf_pos] = '\0';

			k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT);
			rx_buf_pos = 0;
		} else if ((rx_buf_pos < ((int)sizeof(rx_buf)) - 1) && (c != '\n' && c != '\r')) {
			rx_buf[rx_buf_pos++] = c;
		}
	}
}

void console_thread(void *p1, void *p2, void *p3) {

    int ret = uart_irq_callback_user_data_set(uart0, serial_cb, NULL);
    if (ret < 0) {
		if (ret == -ENOTSUP) {
			printk("Interrupt-driven UART API support not enabled\n");
		} else if (ret == -ENOSYS) {
			printk("UART device does not support interrupt-driven API\n");
		} else {
			printk("Error setting UART callback: %d\n", ret);
		}
		return;
	}
    uart_irq_rx_enable(uart0);
    
    char buf[64];

	auto &em = EventMachine::get_machine();
	sys_event_s speed_event{
		.event_type = EV_SPEED,
		.payload { .int_p = 0 }
	};

	sys_event_s gear_event{
		.event_type = EV_GEAR,
		.payload { .int_p = 0 }
	};

	sys_event_s shift_event{
		.event_type = EV_SHIFT,
		.payload { .int_p = 0 }
	};

	sys_event_s rpm_pct_event{
		.event_type = EV_RPM_PCT,
		.payload { .int_p = 0 }
	};

	sys_event_s rpm_event{
		.event_type = EV_RPM,
		.payload { .int_p = 0 }
	};

	while (1) {
		if (k_msgq_get(&uart_msgq, &buf, K_NO_WAIT) == 0) {
			char *p[DATA_COUNT];
			
			p[0] = strtok(buf, ";");

			if (p[0] == NULL || p[0][0] != 'S' || p[0][1] != 'H') { continue; }

			for (int i = 1; i < DATA_COUNT; i++) {
				p[i] = strtok(NULL, ";");
			}

			if (p[1] != NULL) {
				if (p[1][0] == 'N' || p[1][0] == 'n') {
					gear_event.payload.int_p = 0;
				} else if (p[1][0] == 'R' || p[1][0] == 'r') {
					gear_event.payload.int_p = -1;
				} else {
					gear_event.payload.int_p = (int) strtol(p[1], (char **)NULL, 10);
				}
				em.call(gear_event);				
			}
			
			if (p[2] != NULL) {
				speed_event.payload.int_p = (int) strtol(p[2], (char **)NULL, 10);  
				em.call(speed_event);				
			}

			if (p[3] != NULL && p[4] != NULL) {
				shift_event.payload.int_p = (int) strtol(p[3], (char **)NULL, 10) + (int) strtol(p[4], (char **)NULL, 10);  
				em.call(shift_event);		
			}

			if (p[5] != NULL) {
				rpm_pct_event.payload.int_p = (int) strtol(p[5], (char **)NULL, 10);  
				em.call(rpm_pct_event);				
			}

			if (p[6] != NULL) {
				rpm_event.payload.int_p = (int) strtol(p[6], (char **)NULL, 10);  
				em.call(rpm_event);				
			}
		}
		k_sleep(K_MSEC(10));
	}
}
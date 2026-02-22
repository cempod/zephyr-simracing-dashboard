#include "console.h"
#include <stdlib.h>
#include "string.h"
#include <sys/_stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>
#include "event_machine.hpp"

LOG_MODULE_REGISTER(CONSOLE);

const struct device *uart0 = DEVICE_DT_GET(DT_NODELABEL(uart0));
K_MSGQ_DEFINE(uart_msgq, 64, 10, 4);

struct ParseContext {
    char** tokens;
    int count;
    EventMachine& em;
};

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

static int parse_int(const char* str, int default_val = 0) {
    if (str == nullptr) return default_val;
    
    char* endptr;
    long val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') {
        return default_val;
    }
    
    if (val < INT_MIN || val > INT_MAX) {
        return default_val;
    }
    
    return (int) val;
}

static sys_event_s create_int_event(sys_event_e event_type, int value) {
    return sys_event_s{
        .event_type = event_type,
        .payload = { .int_p = value }
    };
}

void parse_main(const ParseContext& ctx) {
	if (ctx.count < MSG_MAIN_SIZE) return;

	ctx.em.call(create_int_event(EV_SPEED, parse_int(ctx.tokens[IDX_SPEED])));				
	ctx.em.call(create_int_event(EV_RPM_PCT, parse_int(ctx.tokens[IDX_RPM_PCT])));				
	ctx.em.call(create_int_event(EV_RPM, parse_int(ctx.tokens[IDX_RPM])));
}

void parse_gear(const ParseContext& ctx) {
	if (ctx.count < MSG_GEAR_SIZE) return;

	int gear_value = 0;
	char gear_char = ctx.tokens[IDX_GEAR][0];

	switch (gear_char) {
		case 'N':
        case 'n':
            gear_value = 0;
            break;
        case 'R':
        case 'r':
            gear_value = -1;
            break;
        default:
            gear_value = parse_int(ctx.tokens[IDX_GEAR], 0);
            break;
	}
	ctx.em.call(create_int_event(EV_GEAR, gear_value));
	ctx.em.call(create_int_event(EV_SHIFT, parse_int(ctx.tokens[IDX_SHIFT_ATTENTION]) + parse_int(ctx.tokens[IDX_SHIFT_WARNING])));		
}

void parse_systems(const ParseContext& ctx) {
	if (ctx.count < MSG_SYSTEMS_SIZE) return;

	ctx.em.call(create_int_event(EV_HANDBRAKE, parse_int(ctx.tokens[IDX_HANDBRAKE])));

	int abs_level = parse_int(ctx.tokens[IDX_ABS_LEVEL]);
	int abs_event = parse_int(ctx.tokens[IDX_ABS_EVENT]);
	if (!abs_event && !abs_level) {
		abs_event = -1;
	}
	ctx.em.call(create_int_event(EV_ABS, abs_event));

	int esp_level = parse_int(ctx.tokens[IDX_ESP_LEVEL]);
	int esp_event = parse_int(ctx.tokens[IDX_ESP_EVENT]);
	if (!esp_level && !esp_event) {
		esp_event = -1;
	}
	ctx.em.call(create_int_event(EV_ESP, esp_event));
}

void parse_lights(const ParseContext& ctx) {
	if (ctx.count < MSG_LIGHTS_SIZE) return;

	int left_turn = parse_int(ctx.tokens[IDX_TURN_LEFT]);
	int right_turn = parse_int(ctx.tokens[IDX_TURN_RIGHT]);

	ctx.em.call(create_int_event(EV_TURN, left_turn + (right_turn << 1)));

	int low_beam = parse_int(ctx.tokens[IDX_BEAM_LOW]);
	int high_beam = parse_int(ctx.tokens[IDX_BEAM_HIGH]);

	ctx.em.call(create_int_event(EV_BEAM, low_beam + (high_beam << 1)));
}

void parse_fuel(const ParseContext& ctx) {
	if (ctx.count < MSG_FUEL_SIZE) return;

	ctx.em.call(create_int_event(EV_FUEL_PCT, parse_int(ctx.tokens[IDX_FUEL_PCT])));
	ctx.em.call(create_int_event(EV_FUEL_ALARM, parse_int(ctx.tokens[IDX_FUEL_ALARM])));
	ctx.em.call(create_int_event(EV_FUEL_VALUE, parse_int(ctx.tokens[IDX_EV_FUEL_VALUE])));
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
	
	while (1) {
		if (k_msgq_get(&uart_msgq, &buf, K_NO_WAIT) == 0) {

			constexpr int MAX_TOKENS = 10;
			char *tokens[MAX_TOKENS];
			int token_count = 0;
			
			char* token = strtok(buf, ";");
            while (token != NULL && token_count < MAX_TOKENS) {
                tokens[token_count++] = token;
                token = strtok(NULL, ";");
            }

			if (token_count < 2 || tokens[0][0] != 'S' || tokens[0][1] != 'H') { continue; }

			const ParseContext ctx = {
				.tokens = tokens,
				.count = token_count,
				.em = em
			};

			int msg_id = parse_int(tokens[1]);

			switch (msg_id) {
				case MSG_MAIN: parse_main(ctx); break;
				case MSG_GEAR: parse_gear(ctx); break;
				case MSG_SYSTEMS: parse_systems(ctx); break;
				case MSG_LIGHTS: parse_lights(ctx); break;
				case MSG_FUEL: parse_fuel(ctx); break;
				default: break;
			}
		}
		k_sleep(K_MSEC(10));
	}
}
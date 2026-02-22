#ifndef CONSOLE_H_
#define CONSOLE_H_

#ifdef __cplusplus
extern "C" {
#endif

enum MessageId {
    MSG_MAIN = 1,
    MSG_GEAR = 2,
    MSG_SYSTEMS = 3,
    MSG_LIGHTS = 4,
    MSG_FUEL = 5
};

enum MessageSize {
    MSG_MAIN_SIZE = 5,
    MSG_GEAR_SIZE = 5,
    MSG_SYSTEMS_SIZE = 7,
    MSG_LIGHTS_SIZE = 6,
    MSG_FUEL_SIZE = 5
};

enum TokenIndex {
    IDX_HEADER = 0,
    IDX_MSG_ID = 1,
    IDX_SPEED = 2,
    IDX_RPM = 3,
    IDX_RPM_PCT = 4,
    IDX_GEAR = 2,
    IDX_SHIFT_ATTENTION = 3,
    IDX_SHIFT_WARNING = 4,
    IDX_ABS_LEVEL = 2,
    IDX_ABS_EVENT = 3,
    IDX_ESP_LEVEL = 4,
    IDX_ESP_EVENT = 5,
    IDX_HANDBRAKE = 6,
    IDX_TURN_LEFT = 2,
    IDX_TURN_RIGHT = 3,
    IDX_BEAM_LOW = 4,
    IDX_BEAM_HIGH = 5,
    IDX_FUEL_PCT = 2,
    IDX_FUEL_ALARM = 3,
    IDX_EV_FUEL_VALUE = 4
};

void console_thread(void *p1, void *p2, void *p3);

#ifdef __cplusplus
}
#endif

#endif
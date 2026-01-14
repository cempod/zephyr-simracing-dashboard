#pragma once

#include <lvgl.h>
#include "event_machine.hpp"

class Dash {
    public:
        static Dash& get_dash();
        void load();
        void update();
    private:
        Dash();
        static void set_speed_callback(sys_event_s event);
        static inline int speed{};
        lv_obj_t * screen;
        lv_obj_t * speed_label;
};
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
        static void set_gear_callback(sys_event_s event);
        static void shift_callback(sys_event_s event);
        static inline int speed{};
        static inline char gear{'N'};
        static inline int shift{};
        lv_obj_t * screen;
        lv_obj_t * gear_label;
        lv_obj_t * speed_label;
        lv_obj_t * speed_label_title;
        lv_obj_t * line_left;
        lv_obj_t * line_right;
        lv_obj_t * line_top;
        lv_obj_t * line_bottom;
};
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
        static Dash* instance;
        #define DECLARE_EVENT(name, sys_event_e, default_value) \
            int name##_ = default_value; \
            static inline void name##_callback(sys_event_s event) { \
                if (instance && event.event_type == sys_event_e) { \
                    instance->name##_ = event.payload.int_p; \
                } \
            } \
        
        DECLARE_EVENT(speed, EV_SPEED, 0);
        DECLARE_EVENT(gear, EV_GEAR, 0);
        DECLARE_EVENT(shift, EV_SHIFT, 0);
        DECLARE_EVENT(rpm, EV_RPM, 0);
        DECLARE_EVENT(rpm_pct, EV_RPM_PCT, 0);
        DECLARE_EVENT(handbrake, EV_HANDBRAKE, 0);
        DECLARE_EVENT(esp, EV_ESP, -1);
        DECLARE_EVENT(abs, EV_ABS, -1);
        DECLARE_EVENT(turn_lights, EV_TURN, 0);
        DECLARE_EVENT(beam, EV_BEAM, 0);
        DECLARE_EVENT(fuel_pct, EV_FUEL_PCT, 0);
        DECLARE_EVENT(fuel_alarm, EV_FUEL_ALARM, 0);
    
        #undef DECLARE_EVENT
        
        lv_obj_t * screen;
        lv_obj_t * gear_label;
        lv_obj_t * speed_label;
        lv_obj_t * speed_label_title;
        lv_obj_t * line_left;
        lv_obj_t * line_right;
        lv_obj_t * line_top;
        lv_obj_t * line_bottom;
        lv_obj_t * rpm_bar;
        lv_obj_t * rpm_label;
        lv_obj_t * rpm_label_title;
        lv_obj_t * abs_img;
        lv_obj_t * esp_img;
        lv_obj_t * brake_img;
        lv_obj_t * fuel_img;
        lv_obj_t * fuel_bar;
        lv_obj_t * left_turn_img;
        lv_obj_t * right_turn_img;
};
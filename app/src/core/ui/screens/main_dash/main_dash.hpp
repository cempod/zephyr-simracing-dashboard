#pragma once

#include "event_machine.hpp"
#include "screen_base.hpp"

class MainDash : public ScreenBase {
    public:
        static MainDash& get();
        void update_callbacks() override;
        void update() override;
    private:
        MainDash();
        static MainDash* instance;
        
        virtual ~MainDash() = default;

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
        DECLARE_EVENT(fuel_value, EV_FUEL_VALUE, 0);
        
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
        lv_obj_t * fuel_label;
        lv_obj_t * left_turn_img;
        lv_obj_t * right_turn_img;
};
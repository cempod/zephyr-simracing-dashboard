#pragma once

#include <lvgl.h>
#include "event_machine.hpp"
#include "misc/lv_color.h"

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
        static void rpm_pct_callback(sys_event_s event);
        static void rpm_callback(sys_event_s event);
        static void handbrake_callback(sys_event_s event);
        static void esp_callback(sys_event_s event);
        static void abs_callback(sys_event_s event);
        static inline int speed{};
        static inline int gear{};
        static inline int shift{};
        static inline int rpm_pct{};
        static inline int rpm{};
        static inline int handbrake{};
        static inline int esp{-1};
        static inline int abs{-1};
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

        lv_color_t color_main = lv_color_make(255, 255, 255);
        lv_color_t color_disabled = lv_color_make(50, 50, 50);
        lv_color_t color_attention = lv_color_make(255, 255, 0);
        lv_color_t color_warning = lv_color_make(255, 0, 0);
        lv_color_t color_green = lv_color_make(0, 255, 0);
};
#include "dash.hpp"
#include "core/lv_obj.h"
#include "event_machine.hpp"
#include "font/lv_font.h"
#include "misc/lv_area.h"
#include "widgets/lv_label.h"

LV_FONT_DECLARE(digits)

static inline void set_label_text(lv_obj_t * label, const char* msg, ...) {
    char str[50];
    va_list ap;
    va_start(ap, msg);
    vsprintf(str, msg, ap);
    va_end(ap);
    lv_label_set_text(label, str);
}

Dash& Dash::get_dash() {
    static Dash dash;
    return dash;
}

Dash::Dash() {
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_make(0, 0, 0), 0);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    gear_label= lv_label_create(screen);
    lv_label_set_text(gear_label, "N");
    lv_obj_set_style_text_font(gear_label, &digits, 0);
    lv_obj_set_style_text_align(gear_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(gear_label, lv_color_make(255, 255, 255), 0);
    lv_obj_align(gear_label, LV_ALIGN_BOTTOM_MID, 0, -13);

    speed_label = lv_label_create(screen);
    lv_label_set_text(speed_label, "0");
    lv_obj_set_style_text_font(speed_label, &digits, 0);
    lv_obj_set_style_text_align(speed_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(speed_label, lv_color_make(255, 255, 255), 0);
    lv_obj_align(speed_label, LV_ALIGN_CENTER, 0, 0);

    speed_label_title = lv_label_create(screen);
    lv_label_set_text(speed_label_title, "Km/h");
    lv_obj_set_style_text_font(speed_label_title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_align(speed_label_title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(speed_label_title, lv_color_make(255, 255, 255), 0);
    lv_obj_align(speed_label_title, LV_ALIGN_CENTER, 0, 50);

    static lv_point_t line_left_points[] = { {0, 0 }, {99, 99}, {99, 220}, {0, 319} };
    static lv_point_t line_right_points[] = { {479, 0 }, {380, 99}, {380, 220}, {479, 319} };
    static lv_point_t line_top_points[] = { {99, 99 }, {380, 99} };
    static lv_point_t line_bottom_points[] = { {99, 220 }, {380, 220} };

    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, lv_color_make(255, 255, 255));
    lv_style_set_line_rounded(&style_line, true);

    line_left = lv_line_create(screen);
    lv_line_set_points(line_left, line_left_points, 4);
    lv_obj_add_style(line_left, &style_line, 0);

    line_right = lv_line_create(screen);
    lv_line_set_points(line_right, line_right_points, 4);
    lv_obj_add_style(line_right, &style_line, 0);

    line_top = lv_line_create(screen);
    lv_line_set_points(line_top, line_top_points, 2);
    lv_obj_add_style(line_top, &style_line, 0);

    line_bottom = lv_line_create(screen);
    lv_line_set_points(line_bottom, line_bottom_points, 2);
    lv_obj_add_style(line_bottom, &style_line, 0);

    auto &em = EventMachine::get_machine();
    em.register_callback(EV_SPEED, set_speed_callback);
    em.register_callback(EV_GEAR, set_gear_callback);
    em.register_callback(EV_SHIFT, shift_callback);
}

void Dash::load() {
    lv_scr_load(screen);
}

void Dash::update() {
    set_label_text(speed_label, "%d", speed);
    set_label_text(gear_label, &gear);
    if (shift == 1) {
        lv_obj_set_style_text_color(gear_label, lv_color_make(255, 255, 0), 0);
    } else if (shift == 2) {
        lv_obj_set_style_text_color(gear_label, lv_color_make(255, 0, 0), 0);
    } else {
        lv_obj_set_style_text_color(gear_label, lv_color_make(255, 255, 255), 0);
    }
}

void Dash::set_speed_callback(sys_event_s event) {
    if (event.event_type == EV_SPEED) {
        speed = event.payload.int_p;
    }
}

void Dash::set_gear_callback(sys_event_s event) {
    if (event.event_type == EV_GEAR) {
        gear = event.payload.char_p;
    }
}

void Dash::shift_callback(sys_event_s event) {
    if (event.event_type == EV_SHIFT) {
        shift = event.payload.int_p;
    }
}
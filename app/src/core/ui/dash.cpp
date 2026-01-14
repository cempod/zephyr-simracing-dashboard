#include "dash.hpp"
#include "core/lv_obj.h"
#include "core/lv_obj_style.h"
#include "event_machine.hpp"
#include "font/lv_font.h"
#include "misc/lv_anim.h"
#include "misc/lv_area.h"
#include "misc/lv_color.h"
#include "widgets/lv_bar.h"
#include "widgets/lv_label.h"

LV_FONT_DECLARE(digits)
LV_FONT_DECLARE(small_digits)

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

    static lv_point_t line_left_points[] = { {0, 0 }, {99, 115}, {99, 220}, {0, 319} };
    static lv_point_t line_right_points[] = { {479, 0 }, {380, 115}, {380, 220}, {479, 319} };
    static lv_point_t line_top_points[] = { {99, 115 }, {380, 115} };
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

    rpm_bar = lv_bar_create(screen);
    lv_obj_set_size(rpm_bar, 260, 40);
    lv_obj_align(rpm_bar, LV_ALIGN_TOP_MID, 0, -20);
    lv_bar_set_range(rpm_bar, 0, 100);
    lv_obj_set_style_bg_opa(rpm_bar, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(rpm_bar, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_border_width(rpm_bar, 2, 0);
    lv_obj_set_style_border_opa(rpm_bar, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(rpm_bar, 2, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(rpm_bar, lv_color_make(255, 255, 255), LV_PART_INDICATOR);
    lv_obj_set_style_pad_all(rpm_bar, 4, 0);

    rpm_label= lv_label_create(screen);
    lv_label_set_text(rpm_label, "0");
    lv_obj_set_style_text_font(rpm_label, &small_digits, 0);
    lv_obj_set_style_text_align(rpm_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(rpm_label, lv_color_make(255, 255, 255), 0);
    lv_obj_align(rpm_label, LV_ALIGN_TOP_MID, 0, 51);

    rpm_label_title = lv_label_create(screen);
    lv_label_set_text(rpm_label_title, "RPM");
    lv_obj_set_style_text_font(rpm_label_title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_align(rpm_label_title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(rpm_label_title, lv_color_make(255, 255, 255), 0);
    lv_obj_align(rpm_label_title, LV_ALIGN_TOP_MID, 0, 28);

    auto &em = EventMachine::get_machine();
    em.register_callback(EV_SPEED, set_speed_callback);
    em.register_callback(EV_GEAR, set_gear_callback);
    em.register_callback(EV_SHIFT, shift_callback);
    em.register_callback(EV_RPM, rpm_callback);
    em.register_callback(EV_RPM_PCT, rpm_pct_callback);
}

void Dash::load() {
    lv_scr_load(screen);
}

void Dash::update() {
    set_label_text(speed_label, "%d", speed);
    if (gear == 0) {
        set_label_text(gear_label, "N");
    } else if (gear == -1) {
        set_label_text(gear_label, "R");
    } else {
        set_label_text(gear_label, "%d", gear);
    }
    
    if (shift == 1) {
        lv_obj_set_style_text_color(gear_label, lv_color_make(255, 255, 0), 0);
    } else if (shift == 2) {
        lv_obj_set_style_text_color(gear_label, lv_color_make(255, 0, 0), 0);
    } else {
        lv_obj_set_style_text_color(gear_label, lv_color_make(255, 255, 255), 0);
    }
    lv_bar_set_value(rpm_bar, rpm_pct, LV_ANIM_OFF);
    set_label_text(rpm_label, "%d", rpm);
}

void Dash::set_speed_callback(sys_event_s event) {
    if (event.event_type == EV_SPEED) {
        speed = event.payload.int_p;
    }
}

void Dash::set_gear_callback(sys_event_s event) {
    if (event.event_type == EV_GEAR) {
        gear = event.payload.int_p;
    }
}

void Dash::shift_callback(sys_event_s event) {
    if (event.event_type == EV_SHIFT) {
        shift = event.payload.int_p;
    }
}

void Dash::rpm_callback(sys_event_s event) {
    if (event.event_type == EV_RPM) {
        rpm = event.payload.int_p;
    }
}

void Dash::rpm_pct_callback(sys_event_s event) {
    if (event.event_type == EV_RPM_PCT) {
        rpm_pct = event.payload.int_p;
    }
}
#include "main_dash.hpp"
#include "core/lv_obj_pos.h"
#include "dash_colors.hpp"
#include "event_machine.hpp"
#include "misc/lv_area.h"
#include "screen_base.hpp"

#include "screen_manager.hpp"

LV_FONT_DECLARE(digit_96)
LV_FONT_DECLARE(digit_64)

MainDash* MainDash::instance = nullptr;

MainDash& MainDash::get() {
    static MainDash dash;
    return dash;
}

static void screen_gesture_event(lv_event_t * e) {
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    switch(dir) {
        case LV_DIR_BOTTOM:
            ScreenManager::get().set_screen(ScreenType::SettingsScreen);
        break;
        default: break;
    }
}

MainDash::MainDash() {
    instance = this;
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_make(0, 0, 0), 0);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(screen, screen_gesture_event, LV_EVENT_GESTURE, NULL);

    gear_label= lv_label_create(screen);
    lv_label_set_text(gear_label, "N");
    lv_obj_set_style_text_font(gear_label, &digit_96, 0);
    lv_obj_set_style_text_align(gear_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(gear_label, DASH_COLOR_MAIN, 0);
    lv_obj_align(gear_label, LV_ALIGN_BOTTOM_MID, 0, -15);

    speed_label = lv_label_create(screen);
    lv_label_set_text(speed_label, "0");
    lv_obj_set_style_text_font(speed_label, &digit_96, 0);
    lv_obj_set_style_text_align(speed_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(speed_label, DASH_COLOR_MAIN, 0);
    lv_obj_align(speed_label, LV_ALIGN_CENTER, 0, 0);

    speed_label_title = lv_label_create(screen);
    lv_label_set_text(speed_label_title, "Km/h");
    lv_obj_set_style_text_font(speed_label_title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_align(speed_label_title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(speed_label_title, DASH_COLOR_MAIN, 0);
    lv_obj_align(speed_label_title, LV_ALIGN_CENTER, 0, 50);

    static lv_point_precise_t line_left_points[] = { {0, 0 }, {99, 115}, {99, 220}, {0, 319} };
    static lv_point_precise_t line_right_points[] = { {479, 0 }, {380, 115}, {380, 220}, {479, 319} };
    static lv_point_precise_t line_top_points[] = { {99, 115 }, {380, 115} };
    static lv_point_precise_t line_bottom_points[] = { {99, 220 }, {380, 220} };

    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, DASH_COLOR_MAIN);
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
    lv_obj_set_style_border_color(rpm_bar, DASH_COLOR_MAIN, 0);
    lv_obj_set_style_border_width(rpm_bar, 2, 0);
    lv_obj_set_style_border_opa(rpm_bar, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(rpm_bar, 2, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(rpm_bar, DASH_COLOR_MAIN, LV_PART_INDICATOR);
    lv_obj_set_style_pad_all(rpm_bar, 4, 0);

    rpm_label= lv_label_create(screen);
    lv_label_set_text(rpm_label, "0");
    lv_obj_set_style_text_font(rpm_label, &digit_64, 0);
    lv_obj_set_style_text_align(rpm_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(rpm_label, DASH_COLOR_MAIN, 0);
    lv_obj_align(rpm_label, LV_ALIGN_TOP_MID, 0, 51);

    rpm_label_title = lv_label_create(screen);
    lv_label_set_text(rpm_label_title, "RPM");
    lv_obj_set_style_text_font(rpm_label_title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_align(rpm_label_title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(rpm_label_title, DASH_COLOR_MAIN, 0);
    lv_obj_align(rpm_label_title, LV_ALIGN_TOP_MID, 0, 28);

    LV_IMG_DECLARE(abs_icon);
    abs_img = lv_img_create(screen);
    lv_img_set_src(abs_img, &abs_icon);
    lv_obj_align(abs_img, LV_ALIGN_LEFT_MID, 5, 65);
    lv_obj_set_size(abs_img, 48, 48);
    lv_obj_set_style_img_recolor_opa(abs_img, LV_OPA_COVER, 0);
    lv_obj_set_style_img_recolor(abs_img, DASH_COLOR_DISABLED, 0);

    LV_IMG_DECLARE(esp_icon);
    esp_img = lv_img_create(screen);
    lv_img_set_src(esp_img, &esp_icon);
    lv_obj_align(esp_img, LV_ALIGN_LEFT_MID, 35, 0);
    lv_obj_set_size(esp_img, 48, 48);
    lv_obj_set_style_img_recolor_opa(esp_img, LV_OPA_COVER, 0);
    lv_obj_set_style_img_recolor(esp_img, DASH_COLOR_DISABLED, 0);

    LV_IMG_DECLARE(brake_icon);
    brake_img = lv_img_create(screen);
    lv_img_set_src(brake_img, &brake_icon);
    lv_obj_align(brake_img, LV_ALIGN_LEFT_MID, 5, -65);
    lv_obj_set_size(brake_img, 48, 48);
    lv_obj_set_style_img_recolor_opa(brake_img, LV_OPA_COVER, 0);
    lv_obj_set_style_img_recolor(brake_img, DASH_COLOR_DISABLED, 0);

    LV_IMG_DECLARE(fuel_icon);
    fuel_img = lv_img_create(screen);
    lv_img_set_src(fuel_img, &fuel_icon);
    lv_obj_align(fuel_img, LV_ALIGN_RIGHT_MID, -35, 0);
    lv_obj_set_size(fuel_img, 48, 48);
    lv_obj_set_style_img_recolor_opa(fuel_img, LV_OPA_COVER, 0);
    lv_obj_set_style_img_recolor(fuel_img, DASH_COLOR_MAIN, 0);

    fuel_bar = lv_bar_create(screen);
    lv_obj_set_size(fuel_bar, 40, 200);
    lv_obj_align(fuel_bar, LV_ALIGN_RIGHT_MID, 20, 0);
    lv_bar_set_range(fuel_bar, 0, 100);
    lv_obj_set_style_bg_opa(fuel_bar, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(fuel_bar, DASH_COLOR_MAIN, 0);
    lv_obj_set_style_border_width(fuel_bar, 2, 0);
    lv_obj_set_style_border_opa(fuel_bar, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(fuel_bar, 2, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(fuel_bar, DASH_COLOR_MAIN, LV_PART_INDICATOR);
    lv_obj_set_style_pad_all(fuel_bar, 4, 0);
    lv_bar_set_value(fuel_bar, 0, LV_ANIM_OFF);
    
    fuel_label= lv_label_create(screen);
    lv_label_set_text(fuel_label, "0L");
    lv_obj_set_style_text_font(fuel_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_align(fuel_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(fuel_label, DASH_COLOR_MAIN, 0);
    lv_obj_align(fuel_label, LV_ALIGN_CENTER, 180, 36);

    LV_IMG_DECLARE(left_turn_icon);
    left_turn_img = lv_img_create(screen);
    lv_img_set_src(left_turn_img, &left_turn_icon);
    lv_obj_align(left_turn_img, LV_ALIGN_TOP_LEFT, 40, 0);
    lv_obj_set_size(left_turn_img, 48, 48);
    lv_obj_set_style_img_recolor_opa(left_turn_img, LV_OPA_COVER, 0);
    lv_obj_set_style_img_recolor(left_turn_img, DASH_COLOR_DISABLED, 0);

    LV_IMG_DECLARE(right_turn_icon);
    right_turn_img = lv_img_create(screen);
    lv_img_set_src(right_turn_img, &right_turn_icon);
    lv_obj_align(right_turn_img, LV_ALIGN_TOP_RIGHT, -40, 0);
    lv_obj_set_size(right_turn_img, 48, 48);
    lv_obj_set_style_img_recolor_opa(right_turn_img, LV_OPA_COVER, 0);
    lv_obj_set_style_img_recolor(right_turn_img, DASH_COLOR_DISABLED, 0);
}

void MainDash::update_callbacks() {
    register_callback(EV_SPEED, speed_callback);
    register_callback(EV_GEAR, gear_callback);
    register_callback(EV_SHIFT, shift_callback);
    register_callback(EV_RPM, rpm_callback);
    register_callback(EV_RPM_PCT, rpm_pct_callback);
    register_callback(EV_HANDBRAKE, handbrake_callback);
    register_callback(EV_ESP, esp_callback);
    register_callback(EV_ABS, abs_callback);
    register_callback(EV_TURN, turn_lights_callback);
    register_callback(EV_FUEL_PCT, fuel_pct_callback);
    register_callback(EV_FUEL_ALARM, fuel_alarm_callback);
    register_callback(EV_FUEL_VALUE, fuel_value_callback);
}

void MainDash::update() {
    set_label_text(speed_label, "%d", speed_);
    
    if (gear_ == 0) {
        set_label_text(gear_label, "N");
    } else if (gear_ == -1) {
        set_label_text(gear_label, "R");
    } else {
        set_label_text(gear_label, "%d", gear_);
    }

    if (shift_ == 1) {
        lv_obj_set_style_text_color(gear_label, DASH_COLOR_ATTENTION, 0);
    } else if (shift_ == 2) {
        lv_obj_set_style_text_color(gear_label, DASH_COLOR_WARNING, 0);
    } else {
        lv_obj_set_style_text_color(gear_label, DASH_COLOR_MAIN, 0);
    }

    set_label_text(rpm_label, "%d", rpm_);

    lv_bar_set_value(rpm_bar, rpm_pct_, LV_ANIM_OFF);

    if (handbrake_ == 1) {
        lv_obj_set_style_img_recolor(brake_img, DASH_COLOR_WARNING, 0);
    } else {
        lv_obj_set_style_img_recolor(brake_img, DASH_COLOR_DISABLED, 0);
    }

    if (esp_ < 0) {
        lv_obj_set_style_img_recolor(esp_img, DASH_COLOR_DISABLED, 0);
    } else if (esp_ == 0) {
        lv_obj_set_style_img_recolor(esp_img, DASH_COLOR_MAIN, 0);
    } else {
        lv_obj_set_style_img_recolor(esp_img, DASH_COLOR_ATTENTION, 0);
    }

    if (abs_ < 0) {
        lv_obj_set_style_img_recolor(abs_img, DASH_COLOR_DISABLED, 0);
    } else if (abs_ == 0) {
        lv_obj_set_style_img_recolor(abs_img, DASH_COLOR_MAIN, 0);
    } else {
        lv_obj_set_style_img_recolor(abs_img, DASH_COLOR_ATTENTION, 0);
    }

    if (turn_lights_ & 0x01) {
        lv_obj_set_style_img_recolor(left_turn_img, DASH_COLOR_GREEN, 0);
    } else {
        lv_obj_set_style_img_recolor(left_turn_img, DASH_COLOR_DISABLED, 0);
    }
    if (turn_lights_ & 0x02) {
        lv_obj_set_style_img_recolor(right_turn_img, DASH_COLOR_GREEN, 0);
    } else {
        lv_obj_set_style_img_recolor(right_turn_img, DASH_COLOR_DISABLED, 0);
    }

    lv_bar_set_value(fuel_bar, fuel_pct_, LV_ANIM_OFF);

    if (fuel_alarm_ > 0) {
        lv_obj_set_style_img_recolor(fuel_img, DASH_COLOR_WARNING, 0);
        lv_obj_set_style_text_color(fuel_label, DASH_COLOR_WARNING, 0);
    } else {
        lv_obj_set_style_img_recolor(fuel_img, DASH_COLOR_MAIN, 0);
        lv_obj_set_style_text_color(fuel_label, DASH_COLOR_MAIN, 0);
    }

    set_label_text(fuel_label, "%dL", fuel_value_);
}
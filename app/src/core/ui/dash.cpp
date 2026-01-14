#include "dash.hpp"

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
    speed_label = lv_label_create(screen);
    lv_label_set_text(speed_label, "0");
    lv_obj_set_style_text_font(speed_label, &digits, 0);
    lv_obj_set_style_text_align(speed_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(speed_label, lv_color_make(255, 255, 255), 0);
    lv_obj_align(speed_label, LV_ALIGN_CENTER, 0, 0);
    auto &em = EventMachine::get_machine();
    em.register_callback(EV_CHANGE_SPEED, set_speed_callback);
}

void Dash::load() {
    lv_scr_load(screen);
}

void Dash::update() {
    set_label_text(speed_label, "%d", speed);
}

void Dash::set_speed_callback(sys_event_s event) {
    if (event.event_type == EV_CHANGE_SPEED) {
        speed = event.payload.int_p;
    }
}
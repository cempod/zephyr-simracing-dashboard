#include "settings_screen.hpp"
#include "dash_colors.hpp"
#include "screen_manager.hpp"

LV_FONT_DECLARE(digits)
LV_FONT_DECLARE(small_digits)

SettingsScreen* SettingsScreen::instance = nullptr;

SettingsScreen& SettingsScreen::get() {
    static SettingsScreen screen;
    return screen;
}

static void screen_gesture_event(lv_event_t * e) {
    lv_obj_t * screen = lv_event_get_current_target(e);
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    switch(dir) {
        case LV_DIR_TOP:
            ScreenManager::get().set_screen(ScreenType::MainDash);
        break;
    }
}

SettingsScreen::SettingsScreen() {
    instance = this;
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_make(0, 0, 0), 0);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(screen, screen_gesture_event, LV_EVENT_GESTURE, NULL);
}

void SettingsScreen::update_callbacks() {}

void SettingsScreen::update() {}
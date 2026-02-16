#include "screen_manager.hpp"
#include "logo_screen.hpp"
#include "screen_base.hpp"

ScreenManager& ScreenManager::get() {
    static ScreenManager manager;
    return manager;
}

ScreenManager::ScreenManager() {
    current_screen = &LogoScreen::get();
    current_screen->show();
}

void ScreenManager::set_screen(ScreenType type) {
    ScreenBase* new_screen;
    switch(type) {
        case ScreenType::MainDash: {
            new_screen = &MainDash::get();
            break;
        }
        case ScreenType::SettingsScreen: {
            new_screen = &SettingsScreen::get();
            break;
        }
        default: return;
    }
    new_screen->show();
    current_screen->detach();
    current_screen = new_screen;
}
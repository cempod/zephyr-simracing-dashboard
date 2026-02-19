#include "screen_manager.hpp"
#include "logo_screen.hpp"
#include "param_machine.hpp"
#include "screen_base.hpp"
#include "param_machine.hpp"

ScreenManager& ScreenManager::get() {
    static ScreenManager manager;
    return manager;
}

ScreenManager::ScreenManager() {
    auto &pm = ParamMachine::get_machine();
    if (pm.get_startup_logo()) {
        current_screen = &LogoScreen::get();
    } else {
        current_screen = &MainDash::get();
    }
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
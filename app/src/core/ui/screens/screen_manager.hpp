#pragma once

#include "screen_base.hpp"
#include "main_dash.hpp"
#include "settings_screen.hpp"

enum class ScreenType { MainDash, SettingsScreen };

class ScreenManager {
    public:
        static ScreenManager& get();

        void update(){
            if (current_screen) current_screen->update();
        }

        void set_screen(ScreenType type);
    private:
        ScreenManager();

        ScreenBase* current_screen = nullptr;
};
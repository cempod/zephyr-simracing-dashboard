#pragma once

#include "screen_base.hpp"

class LogoScreen : public ScreenBase {
    public:
        static LogoScreen& get();
        void update_callbacks() override;
        void update() override;
    private:
        LogoScreen();
        static LogoScreen* instance;
        
        virtual ~LogoScreen() = default;

        lv_obj_t * logo;
};
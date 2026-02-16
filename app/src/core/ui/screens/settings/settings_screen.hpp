#pragma once

#include "screen_base.hpp"

class SettingsScreen : public ScreenBase {
    public:
        static SettingsScreen& get();
        void update_callbacks() override;
        void update() override;
    private:
        SettingsScreen();
        static SettingsScreen* instance;
        
        virtual ~SettingsScreen() = default;

        DECLARE_EVENT(speed, EV_SPEED, 0);
        DECLARE_EVENT(gear, EV_GEAR, 0);
        DECLARE_EVENT(shift, EV_SHIFT, 0);
        DECLARE_EVENT(rpm, EV_RPM, 0);
        DECLARE_EVENT(rpm_pct, EV_RPM_PCT, 0);
        DECLARE_EVENT(handbrake, EV_HANDBRAKE, 0);
        DECLARE_EVENT(esp, EV_ESP, -1);
        DECLARE_EVENT(abs, EV_ABS, -1);
        DECLARE_EVENT(turn_lights, EV_TURN, 0);
        DECLARE_EVENT(beam, EV_BEAM, 0);
        DECLARE_EVENT(fuel_pct, EV_FUEL_PCT, 0);
        DECLARE_EVENT(fuel_alarm, EV_FUEL_ALARM, 0);
};
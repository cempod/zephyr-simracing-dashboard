cmake_minimum_required(VERSION 3.20)

set(SRCS
    ${CMAKE_CURRENT_LIST_DIR}/backlight.cpp
    ${CMAKE_CURRENT_LIST_DIR}/event_machine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/ui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/screens/screen_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/screens/main_dash/main_dash.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/screens/settings/settings_screen.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/fonts/digits.c
    ${CMAKE_CURRENT_LIST_DIR}/ui/fonts/small_digits.c
    ${CMAKE_CURRENT_LIST_DIR}/ui/icons/abs_icon.c
    ${CMAKE_CURRENT_LIST_DIR}/ui/icons/brake_icon.c
    ${CMAKE_CURRENT_LIST_DIR}/ui/icons/esp_icon.c
    ${CMAKE_CURRENT_LIST_DIR}/ui/icons/fuel_icon.c
    ${CMAKE_CURRENT_LIST_DIR}/ui/icons/left_turn_icon.c
    ${CMAKE_CURRENT_LIST_DIR}/ui/icons/right_turn_icon.c
    ${CMAKE_CURRENT_LIST_DIR}/console.cpp
)

add_library(core INTERFACE)

target_include_directories(core INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/ui
    ${CMAKE_CURRENT_LIST_DIR}/ui/screens
    ${CMAKE_CURRENT_LIST_DIR}/ui/screens/main_dash
    ${CMAKE_CURRENT_LIST_DIR}/ui/screens/settings
)

target_sources(core INTERFACE ${SRCS})
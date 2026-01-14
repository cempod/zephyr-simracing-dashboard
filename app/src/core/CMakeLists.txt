cmake_minimum_required(VERSION 3.20)

set(SRCS
    ${CMAKE_CURRENT_LIST_DIR}/backlight.cpp
    ${CMAKE_CURRENT_LIST_DIR}/event_machine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/ui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/dash.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/digits.c
    ${CMAKE_CURRENT_LIST_DIR}/ui/small_digits.c
    ${CMAKE_CURRENT_LIST_DIR}/console.cpp
)

add_library(core INTERFACE)

target_include_directories(core INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/ui
)

target_sources(core INTERFACE ${SRCS})
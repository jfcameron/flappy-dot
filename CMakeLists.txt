# © 2020 Joseph Cameron - All Rights Reserved

cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/jfc-cmake/jfclib.cmake")

project(flappy
    VERSION 0.9
    DESCRIPTION "yet another flappy bird clone")

add_subdirectory(thirdparty)

add_executable(flappy
    ${CMAKE_CURRENT_SOURCE_DIR}/src/assets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/background.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/background_music_player.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bird.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/city.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cloud.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/flappy_event_bus.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/flappy_screen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game_screen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main_menu_screen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/options_screen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pipe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/text/dynamic_text_renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/text/static_text_renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/text/text_map.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/text/text_renderer.cpp)

target_include_directories(flappy PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_features(flappy PRIVATE cxx_std_20)

target_link_libraries(flappy PRIVATE
    gdkaudio
    gdkgraphics
    gdkinput
    gdktime
    gdkwindow
    jfceventbus)

# Hides the console window when building for windows
#set_target_properties(flappy PROPERTIES 
#    LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")

# © 2020 Joseph Cameron - All Rights Reserved

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(null)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/jfc-cmake/jfclib.cmake")

option(JFC_BUILD_DEMO "Build the demo" OFF)
option(JFC_BUILD_DOCS "Build documentation" OFF)
option(JFC_BUILD_TESTS "Build unit tests" OFF)

add_subdirectory(thirdparty)

jfc_project(executable
    NAME "flappy"
    VERSION 0.9
    DESCRIPTION "yet another flappy bird clone"
    C++_STANDARD 17
    C_STANDARD 90

    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        
        ${gdkaudio_INCLUDE_DIRECTORIES} 
        ${gdkgraphics_INCLUDE_DIRECTORIES} 
        ${gdkinput_INCLUDE_DIRECTORIES} 
        ${gdktextrenderer_INCLUDE_DIRECTORIES} 
        ${jfceventbus_INCLUDE_DIRECTORIES} 
        ${simpleglfw_INCLUDE_DIRECTORIES}

    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include

    LIBRARIES
        ${gdkaudio_LIBRARIES} 
        ${gdkgraphics_LIBRARIES}
        ${gdkinput_LIBRARIES}
        ${gdktextrenderer_LIBRARIES}
        ${jfceventbus_LIBRARIES} 
        ${simpleglfw_LIBRARIES}

    SOURCE_LIST
		${CMAKE_CURRENT_SOURCE_DIR}/src/assets.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/background.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/bird.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/city.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/city.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/cloud.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/flappy_screen.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/game.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/game_screen.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/main_menu_screen.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/menu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/options_screen.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/pipe.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/background_music_player.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/control.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/flappy_event_bus.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp

    DEPENDENCIES
        "gdkaudio"
		"gdkgraphics"
		"gdkinput"
		"gdktextrenderer"
		"jfceventbus"
		"simpleglfw"
)

# Hides the console window when building for windows
#set_target_properties(flappy PROPERTIES 
#    LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")

if (JFC_BUILD_TESTS)
    add_subdirectory(test)
endif()

if (JFC_BUILD_DOCS)
    add_subdirectory(docs)
endif()

# TODO: TOOLS, etc

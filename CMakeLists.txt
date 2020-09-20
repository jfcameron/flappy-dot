# © 2019 Joseph Cameron - All Rights Reserved

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

include("${CMAKE_CURRENT_SOURCE_DIR}/jfc-cmake/jfclib.cmake")

option(JFC_BUILD_DEMO "Build the demo" ON)
option(JFC_BUILD_DOCS "Build documentation" ON)
option(JFC_BUILD_TESTS "Build unit tests" ON)

add_subdirectory(thirdparty)

jfc_project(executable
    NAME "flappy"
    VERSION 0.0
    DESCRIPTION "yet another flappy bird clone"
    C++_STANDARD 17
    C_STANDARD 90
    TYPE STATIC

    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        
        ${gdkgraphics_INCLUDE_DIRECTORIES} 
        ${simpleglfw_INCLUDE_DIRECTORIES}

    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include

    LIBRARIES
#        ${gdkgraphics_LIBRARIES}
#        ${simpleglfw_LIBRARIES}

    DEPENDENCIES
        "gdk-graphics"
        "simpleglfw"

    SOURCE_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

#![[if (JFC_BUILD_DEMO)
    add_subdirectory(demo)
endif()

if (JFC_BUILD_TESTS)
    add_subdirectory(test)
endif()

if (JFC_BUILD_DOCS)
    add_subdirectory(docs)
endif()]]

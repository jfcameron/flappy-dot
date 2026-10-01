# © 2020 Joseph Cameron - All Rights Reserved

set(JFC_BUILD_DEMO OFF CACHE BOOL "" FORCE)
set(JFC_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(JFC_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# gdk-graphics brings gdk-windowing (glfw), gdk-math, gdk-time; the others reuse those targets
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/gdk-graphics")
# gdk-input's magic_enum instantiates a fold expression over its default enum range of 257 values,
# one more than clang's default nesting limit of 256
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fbracket-depth=512>)
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/gdk-input")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/gdk-audio")

# jfc-event-bus is header only
add_library(jfceventbus INTERFACE)
target_include_directories(jfceventbus INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/jfc-event-bus/include")

# © 2020 Joseph Cameron - All Rights Reserved

#[[set(dependencies
    "stb"
)]]

jfc_add_dependencies(${dependencies})

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/gdk-graphics")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/simple-glfw")

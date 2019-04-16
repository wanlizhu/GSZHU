cmake_minimum_required(VERSION 3.1)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC "include")
if (WIN32)
    target_link_libraries(glad PUBLIC "opengl32")
endif()
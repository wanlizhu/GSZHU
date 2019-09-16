cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
include(fabric)

project(GraphicsEngine)
zhu_setup_project(GraphicsEngine)

add_subdirectory(Engine)
add_subdirectory(Launcher)

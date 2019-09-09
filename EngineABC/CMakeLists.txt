cmake_minimum_required(VERSION 2.8.11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(EngineABC)

zhu_setup_project(EngineABC)

add_subdirectory(external)
add_subdirectory(plugins)
add_subdirectory(runtime)
add_subdirectory(runtime/test)

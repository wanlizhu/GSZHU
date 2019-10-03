cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/develop/cmake")
include(CMakeAuxiliary)

project(GraphicsEngine)
cm_setup_project(GraphicsEngine)
if (WIN32)
    set(PACKAGES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/packages/x64-windows-static")
    list(APPEND CMAKE_MODULE_PATH "${PACKAGES_DIR}/share")
else()
    message(FATAL_ERROR "!!! TODO !!!")
endif()

add_subdirectory(runtime)
add_subdirectory(samples)
add_subdirectory(tests)

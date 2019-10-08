cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/develop/cmake")
include(CMakeAuxiliary.cmake)
include(GraphicsEngineAuxiliary.cmake)

if (WIN32)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")
    set(GE_USE_CMAKE_FETCH_CONTENT TRUE CACHE BOOL "Use CMake FetchContent() to download external packages")
    set(GE_PACKAGES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/packages/x64-windows-static" CACHE STRING "Where to find external packages")
else()
    message(FATAL_ERROR "!!! TODO !!!")
endif()

project(GraphicsEngine)
cm_setup_project(GraphicsEngine)

add_subdirectory(runtime)
add_subdirectory(samples)
add_subdirectory(tests)

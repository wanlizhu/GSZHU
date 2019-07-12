cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

project(GraphicsEngine VERSION 1.0.0 LANGUAGES C CXX)

# Global flag to cause add_library to create shared libraries if on
option(BUILD_SHARED_LIBS  "Build Shared Libraries"     ON)
option(BUILD_LAUNCHER     "Build launcher application" OFF)
option(BUILD_UNIT_TEST    "Build launcher application" ON)

# Sets the install directories defined by GNU
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON) # To allow making project folders in IDEs
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_INSTALL_PREFIX "D:/Program Files/GraphicsEngine")




add_subdirectory(External)
add_subdirectory(Engine)
if(BUILD_UNIT_TEST)
    add_subdirectory(UnitTest)
endif()
if(BUILD_LAUNCHER)
    add_subdirectory(Launcher)
endif()
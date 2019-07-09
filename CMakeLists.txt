cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

project(ZHUENGINE VERSION 1.0.0 LANGUAGES C CXX)

# Global flag to cause add_library to create shared libraries if on
option(BUILD_SHARED_LIBS "Build Shared Libraries"    ON)
option(BUILD_TESTS       "Build test applications"   ON)
option(BUILD_EDITOR      "Build editor applications" OFF)
option(BUILD_SAMPLES     "Build sample applications" OFF)
option(ENABLE_D3D12      "Enable D3D12"              ON)
option(ENABLE_VULKAN     "Enable Vulkan"             OFF)

# Sets the install directories defined by GNU
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON) # To allow making project folders in IDEs
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_INSTALL_PREFIX "D:/Program Files/ZHUENGINE")

if(WIN32)
    # UNIX will keep a prefix set as "lib"
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()



add_subdirectory(Engine)
if(BUILD_SAMPLES)
    add_subdirectory(Samples/InfiniteCity)
    add_subdirectory(Samples/CAD)
endif()
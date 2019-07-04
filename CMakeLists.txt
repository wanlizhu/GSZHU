cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

project(ZHUENGINE VERSION 1.0.0 LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build Shared Libraries"    ON)
option(BUILD_TESTS       "Build test applications"   OFF)
option(BUILD_BENCHMARKS  "Build test applications"   OFF)
option(BUILD_EDITOR      "Build editor applications" OFF)
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

if(WIN32)
    # UNIX will keep a prefix set as "lib"
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

# Allows usage of configure time content
include(FetchContent)




set(ZHU_SDK_DLLS)
add_subdirectory(Code/SDK)
add_subdirectory(Code/Engine)
if(BUILD_TESTS)
    add_subdirectory(Code/UnitTest)
endif()
if(BUILD_BENCHMARKS)
    add_subdirectory(Code/Benchmarks)
endif()
if(BUILD_EDITOR)
    add_subdirectory(Code/Editor)
endif()
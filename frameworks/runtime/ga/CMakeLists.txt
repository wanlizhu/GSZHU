cmake_minimum_required(VERSION 3.11)
include(CMakeDependentOption)
include(GNUInstallDirs)

set(GA_PROJECT_NAME GeometricAlgorithm)
project(${GA_PROJECT_NAME} LANGUAGES CXX)

# Customize settings
set(GA_DISABLE_SSE          OFF  CACHE BOOL   "")
set(GA_DISABLE_GPGPU        OFF  CACHE BOOL   "")
set(GA_DISABLE_TEST         OFF  CACHE BOOL   "")
set(GA_USE_EIGEN            ON   CACHE BOOL   "")
set(GA_TARGET_NAME          GA2  CACHE STRING "")
set(GA_TARGET_VERSION       1.0  CACHE STRING "")

set_property(GLOBAL PROPERTY USE_FOLDERS  ON)
set(CMAKE_POSITION_INDEPENDENT_CODE       ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS      ON)
set(CMAKE_CXX_STANDARD                    17)
set(CMAKE_CXX_STANDARD_REQUIRED           ON)
set(GA_EXT_LIBS                           "${CMAKE_CURRENT_SOURCE_DIR}/__libs__")

# Update submodules
execute_process(COMMAND git submodule init
    COMMAND git submodule update
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# Add targets
add_library(${GA_TARGET_NAME} STATIC)

file(GLOB_RECURSE _sources_ "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
foreach(_file_ ${_sources_})
    file(RELATIVE_PATH _relative_ "${CMAKE_CURRENT_SOURCE_DIR}" "${_file_}")
    get_filename_component(_path_ "${_relative_}" PATH)
    string(REPLACE "/" "\\\\" _group_ "${_path_}")
    source_group("${_group_}" FILES "${_file_}")
endforeach()
target_sources(${GA_TARGET_NAME} PRIVATE ${_sources_} "dummy.cpp")

target_compile_definitions(${GA_TARGET_NAME} 
    PUBLIC 
    $<$<BOOL:GA_DISABLE_SSE>:GA_DISABLE_SSE>
    $<$<BOOL:GA_DISABLE_GPGPU>:GA_DISABLE_GPGPU>
    $<$<BOOL:GA_USE_EIGEN>:GA_USE_EIGEN>)
target_include_directories(${GA_TARGET_NAME} 
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

# Install
install(DIRECTORY "include/GA2" DESTINATION "include")

# Add tests
if (NOT GA_DISABLE_TEST)
    add_subdirectory(externals/googletest)
    add_subdirectory(tests)
endif()
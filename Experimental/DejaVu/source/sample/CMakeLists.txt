cmake_minimum_required(VERSION 3.17.1)

project(dejavu-sample)

include(GNUInstallDirs)
include(CheckCXXCompilerFlag)
include(CheckCXXSourceRuns)
include(CMakeParseArguments)

find_package(Threads)
find_package(glog REQUIRED)

add_executable(dejavu-sample "main.cpp")

set_target_properties(dejavu-sample PROPERTIES CXX_STANDARD           17)
set_target_properties(dejavu-sample PROPERTIES CXX_STANDARD_REQUIRED  ON)
set_target_properties(dejavu-sample PROPERTIES CXX_EXTENSIONS         ON)

target_include_directories(dejavu-sample PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(dejavu-sample PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_definitions(dejavu-sample PRIVATE "UNICODE")
target_compile_definitions(dejavu-sample PRIVATE $<$<PLATFORM_ID:Windows>:"_CRT_SECURE_NO_WARNINGS">)

target_link_libraries(dejavu-sample PRIVATE dejavu-core)
target_link_libraries(dejavu-sample PRIVATE glog::glog)

target_sources(dejavu-sample PRIVATE "sample_application.h")
target_sources(dejavu-sample PRIVATE "sample_application.cpp")

if (COMMAND djv_source_group)
    djv_source_group(TARGET dejavu-sample ROOTS "${CMAKE_CURRENT_SOURCE_DIR}")    
endif()
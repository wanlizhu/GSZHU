cmake_minimum_required(VERSION 3.17.1)

project(dejavu-unittest)

include(GNUInstallDirs)
include(CheckCXXCompilerFlag)
include(CheckCXXSourceRuns)
include(CMakeParseArguments)

find_package(GTest REQUIRED)

add_executable(dejavu-unittest "dummy.cpp")

set_target_properties(dejavu-unittest PROPERTIES CXX_STANDARD           17)
set_target_properties(dejavu-unittest PROPERTIES CXX_STANDARD_REQUIRED  ON)
set_target_properties(dejavu-unittest PROPERTIES CXX_EXTENSIONS         ON)

target_include_directories(dejavu-unittest PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(dejavu-unittest PRIVATE "UNICODE")

target_link_libraries(dejavu-unittest PRIVATE GTest::gtest)
target_link_libraries(dejavu-unittest PRIVATE GTest::gtest_main)
target_link_libraries(dejavu-unittest PRIVATE dejavu-core)

target_sources(dejavu-unittest PRIVATE "core-bitmap-test.cpp")
target_sources(dejavu-unittest PRIVATE "core-string-test.cpp")

if (COMMAND djv_source_group)
    djv_source_group(TARGET dejavu-unittest ROOTS "${CMAKE_CURRENT_SOURCE_DIR}")    
endif()
cmake_minimum_required(VERSION 3.15.0)
project(dejavu-unittest)
include(GNUInstallDirs)
include(CheckCXXCompilerFlag)
include(CheckCXXSourceRuns)
include(CMakeParseArguments)

if (DJV_ROOT_DIR)
    set(DJV_UNITTEST_INCLUDE_DIR "${DJV_ROOT_DIR}/include")
else()
    set(DJV_UNITTEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(DJV_USE_EIGEN ON CACHE BOOL "Use Eigen library as the linear algebra backend")

add_executable(dejavu-unittest "dummy.cpp")

find_package(GTest CONFIG REQUIRED)

target_include_directories(dejavu-unittest PRIVATE "${DJV_UNITTEST_INCLUDE_DIR}")
target_include_directories(dejavu-unittest PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(dejavu-unittest PRIVATE "UNICODE")
target_link_libraries(dejavu-unittest PRIVATE GTest::gtest)
target_link_libraries(dejavu-unittest PRIVATE GTest::gtest_main)
target_link_libraries(dejavu-unittest PRIVATE dejavu-core)

target_sources(dejavu-unittest PRIVATE "core-bitmap-test.cpp")
target_sources(dejavu-unittest PRIVATE "core-string-test.cpp")

if (COMMAND djv_source_group)
    djv_source_group(TARGET dejavu-unittest ROOTS "${CMAKE_CURRENT_SOURCE_DIR}" "${DJV_UNITTEST_INCLUDE_DIR}")    
endif()
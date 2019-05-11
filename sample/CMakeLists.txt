cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

## find SDL2
if (NOT DEFINED ENV{SDL2DIR})
    set(ENV{SDL2DIR} "${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty")
endif()
find_package(SDL2 REQUIRED)


file(GLOB_RECURSE SRCS source/*.cpp)
file(GLOB_RECURSE HDRS source/*.h)


add_executable(GSZHUSample ${SRCS} ${HDRS})
add_definitions(-DSDL_MAIN_HANDLED)
include_directories("." "source" ${SDL2_INCLUDE_DIR})
target_link_libraries(GSZHUSample ${GSZHU_ENGINE} ${SDL2_LIBRARY})
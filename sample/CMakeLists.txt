cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

## find SDL2
if (NOT DEFINED ENV{SDL2DIR})
    set(ENV{SDL2DIR} "${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty")
endif()
find_package(SDL2)

file(GLOB_RECURSE SRCS source/*.cpp)
file(GLOB_RECURSE HDRS source/*.h)


add_executable(GSZHUSample ${SRCS} ${HDRS})
add_definitions(-DSDL_MAIN_HANDLED)
if(GSZHU_BUILD_SHARED)
    add_definitions(-DGSZHU_IMPORT_SHARED)
endif()
if(SDL2_FOUND)
    add_definitions(_DGSZHU_USE_SDL2)
    message(STATUS "Use SDL2 library as the window system")
endif()
include_directories("." "source" ${SDL2_INCLUDE_DIR})
target_link_libraries(GSZHUSample ${GSZHU_ENGINE} ${SDL2_LIBRARY})
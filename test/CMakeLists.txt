cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

file(GLOB_RECURSE SRCS source/*.cpp)
file(GLOB_RECURSE HDRS source/*.h)


add_executable(GSZHUTest ${SRCS} ${HDRS})
include_directories("." "source" ${SDL2_INCLUDE_DIR})
target_link_libraries(GSZHUTest ${GSZHU_ENGINE})
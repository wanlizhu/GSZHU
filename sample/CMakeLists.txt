cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

file(GLOB_RECURSE SRCS source/*.cpp)
file(GLOB_RECURSE HDRS source/*.h)

set(APP_NAME "SampleApp")

add_executable(${APP_NAME} ${SRCS} ${HDRS})
if(NOT GSZHU_BUILD_STATIC)
    add_definitions(-DGSZHU_IMPORT_SHARED)
endif()

include_directories("." "source")
target_link_libraries(${APP_NAME} ${GSZHU_ENGINE})
cmake_minimum_required(VERSION 3.1)

if (WIN32)
    add_definitions(/W0)
endif()

include_directories(include .)

add_library(gtest STATIC src/gtest-all.cc)
set_target_properties(gtest PROPERTIES FOLDER ThirdParty)
target_include_directories(gtest PUBLIC include)

if (LINUX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(gtest Threads::Threads)
endif()


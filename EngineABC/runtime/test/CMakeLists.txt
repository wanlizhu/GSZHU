cmake_minimum_required(VERSION 2.8.11)
project(EngineABC_Test)

file(GLOB_RECURSE SourceFiles "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp" 
                              "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")

add_executable(EngineABC_Test ${SourceFiles})
target_link_directories(EngineABC_Test PRIVATE ${EngineABCTarget} External::GTEST)
cmake_minimum_required(VERSION 2.8.11)
project(EngineABC_Test)

file(GLOB_RECURSE SourceFiles "source/*.cpp" "source/*.h")

add_executable(EngineABC_Test ${SourceFiles})
target_include_directories(EngineABC_Test external/gtest/include)
target_compile_definitions(EngineABC_Test UNICODE _UNICODE)
target_link_directories(EngineABC_Test 
	EngineABCTarget
	external/gtest/lib/gtest$<$<CONFIG:Debug>:d>.lib
	external/gtest/lib/gmock$<$<CONFIG:Debug>:d>.lib)
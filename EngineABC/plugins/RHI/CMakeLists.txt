cmake_minimum_required(VERSION 2.8.11)
project(PluginRHI)

file(GLOB_RECURSE SourceFiles "source/*.cpp" "source/*.h")

add_executable(PluginRHI ${SourceFiles})
target_include_directories(PluginRHI external/gtest/include)
target_compile_definitions(PluginRHI UNICODE _UNICODE)
target_link_directories(PluginRHI 
	EngineABCTarget
	external/gtest/lib/gtest$<$<CONFIG:Debug>:d>.lib
	external/gtest/lib/gmock$<$<CONFIG:Debug>:d>.lib)
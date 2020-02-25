cmake_minimum_required(VERSION 3.11)
include(GNUInstallDirs)

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
set(Output_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}")
set(ShaderCompiler_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Engine/SPIRV-Compile.py")

file(GLOB_RECURSE SourceFiles "${CSD}/*.h" "${CSD}/*.cpp")
file(GLOB_RECURSE ShaderFiles "${CSD}/*.glsl")

add_executable(Triangle ${SourceFiles})
set_target_properties(Triangle PROPERTIES FOLDER "Samples")

target_compile_definitions(Triangle PRIVATE "_UNICODE" "UNICODE")
target_link_libraries(Triangle PRIVATE Wanli)

if (EXISTS "Assets")
    add_custom_command(
        TARGET Triangle
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "Assets" "${Output_DIR}"
        WORKING_DIRECTORY "${CSD}"
    )
endif()

if (ShaderFiles)
    add_custom_command(
        TARGET Triangle
        POST_BUILD
        COMMAND python SPIRV-Compile.py --glsl --reflect --output "${Output_DIR}/Shaders" "${ShaderFiles}"
        WORKING_DIRECTORY "${CSD}"
    )
endif()

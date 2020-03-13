cmake_minimum_required(VERSION 3.11)
include(GNUInstallDirs)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(GenerateSample SampleName)
    set(CSD "${CMAKE_CURRENT_SOURCE_DIR}/${SampleName}")
    set(EngineAssets_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Engine/Assets")
    set(Output_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}")
    set(ShaderCompiler_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Engine/SPIRV-Compile.py")

    # Source files
    file(GLOB_RECURSE SourceFiles "${CSD}/*.cpp" "${CSD}/*.h")
    file(GLOB_RECURSE ShaderFiles "${CSD}/*.glsl")

    # Source group
    source_group("Source Files" FILES ${SourceFiles})
    source_group("Shader Files" FILES ${ShaderFiles})

    # Add executable
    add_executable(${SampleName} ${SourceFiles} ${ShaderFiles})
    set_target_properties(${SampleName} PROPERTIES FOLDER "Samples")
    target_link_libraries(${SampleName} Wanli)
    set_target_properties(${SampleName} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${Output_DIR}")
    set_target_properties(${SampleName} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "--search-path \"${CSD};${EngineAssets_DIR}\"")
    target_compile_definitions(${SampleName} PRIVATE "_UNICODE" "UNICODE")
    target_compile_definitions(${SampleName} PRIVATE "${FileSearchPath}")
    target_compile_options(${SampleName} PRIVATE
        "/wd4251" # members of exported class are not exported
        "/wd4275" # non dll-interface class used as base for dll-interface class
        "/wd4996" # when using std::codecvt_utf8_utf16<wchar_t>
    )

    # Copy the original shader or compiled SPIRV shaders
    if (PRECOMPILE_SHADER)
        add_custom_command(
            TARGET ${SampleName}
            POST_BUILD
            COMMAND python "${ShaderCompiler_PATH}" --glsl "${CSD}" --output "${CSD}"
            WORKING_DIRECTORY "${CSD}"
        )
    endif()
endfunction()

file(GLOB children RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*")
foreach(child ${children})
    if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${child}")
        GenerateSample(${child})
    endif()
endforeach()
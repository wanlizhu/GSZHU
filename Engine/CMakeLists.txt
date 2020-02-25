cmake_minimum_required(VERSION 3.11)
include(GNUInstallDirs)

add_subdirectory(ThirdParty)
find_package(Vulkan)

project(Wanli)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
set(Output_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}")
set(ThirdParty_DIR "${CMAKE_CURRENT_BINARY_DIR}/ThirdParty")

add_library(Wanli SHARED)

set_target_properties(Wanli PROPERTIES FOLDER "Engine")

file(GLOB_RECURSE ConfigFiles "${CSD}/Config/*.ini")
file(GLOB_RECURSE SourceFiles "${CSD}/Source/*.h" "${CSD}/Source/*.cpp")
file(GLOB_RECURSE ShaderFiles "${CSD}/Shaders/*.glsl")

source_group(TREE "${CSD}/Config" PREFIX "Config Files" FILES ${ConfigFiles})
source_group(TREE "${CSD}/Source" PREFIX "Source Files" FILES ${SourceFiles})
source_group(TREE "${CSD}/Shaders" PREFIX "Shader Files" FILES ${ShaderFiles})

target_sources(Wanli PRIVATE ${ConfigFiles} ${SourceFiles} ${ShaderFiles})

target_include_directories(Wanli PUBLIC "${CSD}/Source")
target_include_directories(Wanli PRIVATE "${ThirdParty_DIR}")

target_compile_definitions(Wanli PRIVATE "_UNICODE" "UNICODE")
target_compile_definitions(Wanli PRIVATE "ENABLE_ASYNC_LOG")

target_link_libraries(Wanli PUBLIC PKG::glm)
target_link_libraries(Wanli PRIVATE PKG::imgui)
target_link_libraries(Wanli PRIVATE PKG::glfw)
target_link_libraries(Wanli PRIVATE PKG::tinygltf)
target_link_libraries(Wanli PRIVATE PKG::LodePNG)

target_compile_definitions(Wanli PRIVATE "WANLI_BUILD_SHARED_LIB")

if (ENABLE_VULKAN)
    target_compile_definitions(Wanli PRIVATE "ENABLE_VULKAN")

    target_link_libraries(Wanli PRIVATE Vulkan::Vulkan)
endif()

if (WINDOWS OR (WIN32 AND NOT ANDROID))
    set(WindowsCompileDefinitions
        "PLATFORM_WINDOWS=1"
        "WIN32_LEAN_AND_MEAN"
        "_SCL_SECURE_NO_WARNINGS"
        "_CRT_SECURE_NO_WARNINGS"
        "_WINSOCK_DEPRECATED_NO_WARNINGS"
        "NOMINMAX"
    )
    target_compile_definitions(Wanli PRIVATE ${WindowsCompileDefinitions})

    set(WindowsCompileOptions
        "/wd4251" # members of exported class are not exported
        "/wd4996" # when using std::codecvt_utf8_utf16<wchar_t>
    )
    target_compile_options(Wanli PRIVATE ${WindowsCompileOptions}) 

    if (ENABLE_VULKAN)
        target_compile_definitions(Wanli PRIVATE "VK_USE_PLATFORM_WIN32_KHR")
    endif()
elseif (ANDROID)
else()
    message(FATAL_ERROR "!!! Undefined target platform !!!")
endif()

if (MSVC)
    set(_PCH_HeaderFile_ "stdafx.h")
    set(_PCH_SourceFile_ "${CSD}/Source/stdafx.cpp")

    set_target_properties(Wanli PROPERTIES 
        COMPILE_FLAGS "/Yu${_PCH_HeaderFile_} /Fi${_PCH_HeaderFile_} /Fp${Output_DIR}/stdafx.pch")
    set_source_files_properties("${_PCH_SourceFile_}" PROPERTIES 
        COMPILE_FLAGS "/Yc${_PCH_HeaderFile_}")
endif()

add_custom_command(
    TARGET Wanli
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${ConfigFiles}  "${Output_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy ${PKG_BINS}  "${Output_DIR}"
    WORKING_DIRECTORY "${CSD}"
)

add_custom_command(
    TARGET Wanli
    POST_BUILD
    COMMAND python SPIRV-Compile.py --glsl --reflect --output "${Output_DIR}/Shaders" "${CSD}/Shaders"
    WORKING_DIRECTORY "${CSD}"
)
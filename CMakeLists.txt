cmake_minimum_required(VERSION 3.11)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CMakeParseArguments)
include(GNUInstallDirs)

project(GraphicsEngine)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(WANLIX_OUTPUT_WITH_CONFIG_SUBDIR OFF CACHE BOOL "")
set(WANLIX_CODE_REVIEW_MODE OFF CACHE BOOL "")
set(BUILD_SHARED_LIBS       ON  CACHE BOOL "")
set(WANLIX_BUILD_EDITOR     ON  CACHE BOOL "")
set(WANLIX_BUILD_TEST       ON  CACHE BOOL "")
set(WANLIX_ENABLE_VULKAN    ON  CACHE BOOL "")
set(WANLIX_ENABLE_D3D12     ON  CACHE BOOL "")
set(WANLIX_ENABLE_METAL     ON  CACHE BOOL "")
set(WANLIX_ENABLE_OPENGL    ON  CACHE BOOL "")
set(WANLIX_USE_ASSIMP       ON  CACHE BOOL "")
set(WANLIX_VERSION_MAJOR 1)
set(WANLIX_VERSION_MINOR 0)
set(WANLIX_VERSION_PATCH 0)
set(WANLIX_VERSION "${WANLIX_VERSION_MAJOR}.${WANLIX_VERSION_MINOR}.${WANLIX_VERSION_PATCH}")
set(RSD "${CMAKE_CURRENT_SOURCE_DIR}")
set(RBD "${CMAKE_CURRENT_BINARY_DIR}")

if (WIN32)
    set(WANLIX_ENABLE_METAL OFF)
endif()
if (APPLE)
    set(WANLIX_ENABLE_D3D12 OFF)
endif()
if (NOT WANLIX_CODE_REVIEW_MODE)
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup(TARGETS)
endif()




function (SetTargetOutputDirectories 
    _targetName_
)
    if (NOT WANLIX_OUTPUT_WITH_CONFIG_SUBDIR)
        get_target_property(_outdir_ ${_targetName_} RUNTIME_OUTPUT_DIRECTORY)
        get_target_property(_libdir_ ${_targetName_} LIBRARY_OUTPUT_DIRECTORY)
        set_target_properties(${_targetName_} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${_outdir_}")
        set_target_properties(${_targetName_} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${_outdir_}")
        set_target_properties(${_targetName_} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINISIZEREL    "${_outdir_}")
        set_target_properties(${_targetName_} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${_outdir_}")
        set_target_properties(${_targetName_} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${_libdir_}")
        set_target_properties(${_targetName_} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${_libdir_}")
        set_target_properties(${_targetName_} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_MINISIZEREL    "${_libdir_}")
        set_target_properties(${_targetName_} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${_libdir_}")
    endif()
endfunction()

function (GetCurrentDirectorySourceFiles 
    _interfaceFiles_ 
    _headerFiles_ 
    _sourceFiles_
)
    set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
    set(COD "${CMAKE_CURRENT_BINARY_DIR}")

    file(GLOB_RECURSE ${_interfaceFiles_} "${CSD}/include/*.h" "${CSD}/include/*.hpp")
    file(GLOB_RECURSE _classesFiles_      "${CSD}/classes/*.h" "${CSD}/classes/*.hpp")
    file(GLOB_RECURSE ${_headerFiles_}    "${CSD}/source/*.h"  "${CSD}/source/*.hpp")
    file(GLOB_RECURSE ${_sourceFiles_}    "${CSD}/source/*.c"  "${CSD}/source/*.cpp")
    if (APPLE)
        file(GLOB_RECURSE _objc_ "${CSD}/source/*.m" "${CSD}/source/*.mm")
        list(APPEND ${_sourceFiles_} ${_objc_})
    endif()

    source_group(TREE "${CSD}/include" PREFIX "Header Files" FILES ${${_interfaceFiles_}})
    source_group(TREE "${CSD}/classes" PREFIX "Class Files" FILES ${_classesFiles_})
    source_group(TREE "${CSD}/source"  PREFIX "Source Files" FILES ${${_headerFiles_}})
    source_group(TREE "${CSD}/source"  PREFIX "Source Files" FILES ${${_sourceFiles_}})

    set(${_interfaceFiles_} ${${_interfaceFiles_}} PARENT_SCOPE)
    set(${_headerFiles_}    ${${_headerFiles_}} ${_classesFiles_} PARENT_SCOPE)
    set(${_sourceFiles_}    ${${_sourceFiles_}} PARENT_SCOPE)
endfunction()




add_subdirectory(runtime)
add_library(Wanlix SHARED dummy.cpp)

SetTargetOutputDirectories(Wanlix)
GetCurrentDirectorySourceFiles(_interfaces_ _headers_ _sources_)
target_sources(Wanlix PRIVATE ${_interfaces_} ${_headers_} ${_sources_})
set_target_properties(Wanlix PROPERTIES VERSION ${WANLIX_VERSION})

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(Wanlix PUBLIC "${CSD}/include")
target_include_directories(Wanlix PRIVATE "${CSD}/source")
target_link_libraries(Wanlix PRIVATE RHI)
target_link_libraries(Wanlix PRIVATE GAL)
target_compile_definitions(Wanlix PRIVATE "_UNICODE" "UNICODE")

if (WANLIX_BUILD_EDITOR)
    add_subdirectory(editor)
endif()

get_target_property(_outdir_ Wanlix RUNTIME_OUTPUT_DIRECTORY)
message(STATUS "Target: Wanlix -> ${_outdir_}" )
add_custom_command(
    TARGET Wanlix 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CSD}/data" "${_outdir_}/data"
    WORKING_DIRECTORY "${CSD}"
)

message(${CMAKE_SYSTEM_NAME})
message(${CMAKE_SYSTEM_PROCESSOR})
message(${CMAKE_SYSTEM_VERSION})
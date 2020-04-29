cmake_minimum_required(VERSION 3.17.1)

execute_process(
    COMMAND python3 setup-vcpkg.py 
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# The CMAKE_TOOLCHAIN_FILE setting simply must be set before the project() directive is first called
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if (NOT DEFINED ENV{VCPKG_ROOT})
        if (WIN32)
            set(CMAKE_TOOLCHAIN_FILE "$ENV{HOMEDRIVE}$ENV{HOMEPATH}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "") 
        else()
            set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
        endif()
    else()
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    endif()
    
    file(TO_CMAKE_PATH "${CMAKE_TOOLCHAIN_FILE}" CMAKE_TOOLCHAIN_FILE)
endif()

if (NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_APPLOCAL_DEPS ON CACHE BOOL "")

    if (WIN32)
        set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")
    elseif (APPLE)
        set(VCPKG_TARGET_TRIPLET "x64-osx" CACHE STRING "")
    else (UNIX)
        set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "")
    endif()
endif()

if (EXISTS "${CMAKE_TOOLCHAIN_FILE}")
    message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
    message(STATUS "VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")
    message(STATUS "VCPKG_APPLOCAL_DEPS:  ${VCPKG_APPLOCAL_DEPS}")
endif()

project(DejaVu)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH            ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
set(CMAKE_CXX_STANDARD           17)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_UNITY_BUILD            OFF CACHE BOOL "")  # Accelerate building process
set(DJV_ROOT_DIR                 "${CMAKE_CURRENT_SOURCE_DIR}")
set(DJV_BUILD_SAMPLES            ON CACHE BOOL "")
set(DJV_BUILD_TESTS              ON CACHE BOOL "")

function(djv_source_group)
    set(Options "")
    set(OneValueArgs TARGET)
    set(MultiValueArgs ROOTS)

    cmake_parse_arguments(DJV_SG "${Options}" "${OneValueArgs}" "${MultiValueArgs}" ${ARGN})

    get_target_property(TargetSources ${DJV_SG_TARGET} SOURCES)
    set(HeaderExtensions ".h" ".hpp")
    set(SourceExtensions ".c" ".cpp" ".m" ".mm" ".cc" ".cxx")
    set(ShaderExtensions ".glsl" ".hlsl" ".vert" ".tesc" ".tese" ".geom" ".frag" ".comp" ".mesh" ".task" ".rgen" ".rint" ".rahit" ".rchit" ".rmiss" ".rcall")
    
    foreach(SourceName ${TargetSources})
        get_filename_component(SourceAbsPath "${SourceName}" ABSOLUTE)
        get_filename_component(SourceExtName "${SourceName}" LAST_EXT)

        # Determine current group name's prefix 
        if ("${SourceExtName}" IN_LIST SourceExtensions)
            set(GroupNamePrefix "Source Files")
        elseif ("${SourceExtName}" IN_LIST HeaderExtensions)
            set(GroupNamePrefix "Header Files")
        elseif ("${SourceExtName}" IN_LIST ShaderExtensions)
            set(GroupNamePrefix "Shader Files")
        else()
            set(GroupNamePrefix "Resource Files")
        endif()

        foreach(CurrentRootName ${DJV_SG_ROOTS})
            file(RELATIVE_PATH SourceRelativePath "${CurrentRootName}" "${SourceAbsPath}")
            string(FIND "${SourceRelativePath}" ".." DualDotsPos)
            
            # Current source file is located under current root directory
            if ("${DualDotsPos}" STREQUAL "-1")
                source_group(TREE "${CurrentRootName}" PREFIX "${GroupNamePrefix}" FILES "${SourceAbsPath}")
            endif()
        endforeach()
    endforeach()
endfunction()

add_subdirectory(source/externals)
add_subdirectory(source/dejavu)
add_subdirectory(source/plugins)

if (DJV_BUILD_SAMPLES)
    add_subdirectory(source/samples)
endif()

if (DJV_BUILD_TESTS)
    add_subdirectory(tests/unit-test)
endif()

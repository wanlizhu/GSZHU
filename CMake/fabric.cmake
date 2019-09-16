set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_DEBUG_POSTFIX "")

include(CMakeParseArguments)
include(GNUInstallDirs)
include(fabric_private)

# One-time initialization of the build system. 
macro(zhu_setup_project _proj)
    if(MSVC OR (CMAKE_GENERATOR STREQUAL "Xcode"))
        set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
        message(STATUS "CMAKE_CONFIGURATION_TYPES: ${CMAKE_CONFIGURATION_TYPES}")
    else()
        # For VS and XCode, CMAKE_BUILD_TYPE on generate step is ignored 
        message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
    endif()

    set_property(GLOBAL PROPERTY USE_FOLDERS ON) # To allow making project folders in IDEs
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

    message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY: " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    message(STATUS "")

    project(${_proj})

    # Should be called after project() when CMAKE_CXX_COMPILER_ID is set
    zhu_set_system_variables()
    zhu_hide_inlines()
    enable_language(ASM_MASM)
endmacro()

macro(zhu_load_target_version)
    set(_major 1)
    set(_minor 0)
    set(_patch 0)

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION")
        file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" _fullVersion)
        string(REPLACE "-" "." _temp ${_fullVersion})
        string(REPLACE "." ";" _versionTokens ${_temp})
        
        list(LENGTH _versionTokens _versionTokensLen)
        list(GET _versionTokens 0 _major)
        if(_versionTokensLen GREATER 1)
            list(GET _versionTokens 1 _minor)
        endif()
        if(_versionTokensLen GREATER 2)
            list(GET _versionTokens 2 _patch)
        endif()
    endif()

    set(${__CurrentTargetVersionMajor__} ${_major})
    set(${__CurrentTargetVersionMinor__} ${_minor})
    set(${__CurrentTargetVersionPatch__} ${_patch})
    set(${__CurrentTargetVersion__}     "${_major}.${_minor}.${_patch}")
endmacro()

macro(zhu_set_output_name _name)
    set(__CurrentTargetOutputName__ ${_name})
endmacro()

macro(zhu_add_resources)
    if(__IsImportedTarget__)
        message(FATAL_ERROR "!!! IMPORTED target is not allowed here !!!")
    endif()
    foreach(_res ${ARGV})
        list(APPEND __CurrentResources__ ${_res})
    endforeach()
endmacro()













macro(zhu_compile_definitions)
    set(_type PRIVATE)
    if(__IsImportedTarget__)
        set(_type INTERFACE)
    endif()

    foreach(_def ${ARGV})
        target_compile_definitions(${__CurrentTargetName__} ${_type} ${_def})
    endforeach()
endmacro()

macro(zhu_compile_definitions_if)
    set(_type PRIVATE)
    if(__IsImportedTarget__)
        set(_type INTERFACE)
    endif()

    foreach(_var ${ARGV})
        if(${_var})
            target_compile_definitions(${__CurrentTargetName__} ${_type} ${_var})
        endif()
    endforeach()
endmacro()

macro(zhu_compile_definitions_public)
    set(_type PUBLIC)
    if(__IsImportedTarget__)
        set(_type INTERFACE)
    endif()

    foreach(_def ${ARGV})
        target_compile_definitions(${__CurrentTargetName__} ${_type} ${_def})
    endforeach()
endmacro()

macro(zhu_compile_definitions_public_if)
    set(_type PUBLIC)
    if(__IsImportedTarget__)
        set(_type INTERFACE)
    endif()

    foreach(_var ${ARGV})
        if(${_var})
            target_compile_definitions(${__CurrentTargetName__} ${_type} ${_var})
        endif()
    endforeach()
endmacro()

macro(zhu_include_directories)
    set(_type PRIVATE)
    if(__IsImportedTarget__)
        set(_type INTERFACE)
    endif()

    foreach(_inc ${ARGV})
        target_include_directories(${__CurrentTargetName__} ${_type} ${_inc})
    endforeach()
endmacro()

macro(zhu_include_directories_public)
    set(_type PUBLIC)
    if(__IsImportedTarget__)
        set(_type INTERFACE)
    endif()

    foreach(_inc ${ARGV})
        target_include_directories(${__CurrentTargetName__} ${_type} ${_inc})
    endforeach()
endmacro()

macro(zhu_link_libraries)
    set(_type PRIVATE)
    if(__IsImportedTarget__)
        set(_type INTERFACE)
    endif()

    foreach(_lib ${ARGV})
        target_link_libraries(${__CurrentTargetName__} ${_type} ${_lib})
    endforeach()
endmacro()

macro(zhu_link_libraries_public)
    set(_type PUBLIC)
    if(__IsImportedTarget__)
        set(_type INTERFACE)
    endif()

    foreach(_lib ${ARGV})
        target_link_libraries(${__CurrentTargetName__} ${type} ${_lib})
    endforeach()
endmacro()

macro(zhu_set_imported_location)
    set(_oneValueArgs DEBUG RELEASE)
    cmake_parse_arguments(_ARGS "" "${_oneValueArgs}" "" ${ARGN})

    if(_ARGS_DEBUG)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                          IMPORTED_LOCATION_DEBUG ${_ARGS_DEBUG})
    elseif(_ARGS_RELEASE)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                          IMPORTED_LOCATION_RELEASE ${_ARGS_RELEASE})
    else()
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                          IMPORTED_LOCATION ${_ARGS_UNPARSED_ARGUMENTS})
    endif()
endmacro()

macro(zhu_set_imported_implib)
    set(_oneValueArgs DEBUG RELEASE)
    cmake_parse_arguments(_ARGS "" "${_oneValueArgs}" "" ${ARGN})

    if(_ARGS_DEBUG)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                          IMPORTED_IMPLIB_DEBUG ${_ARGS_DEBUG})
    elseif(_ARGS_RELEASE)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                          IMPORTED_IMPLIB_RELEASE ${_ARGS_RELEASE})
    else()
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                          IMPORTED_IMPLIB ${_ARGS_UNPARSED_ARGUMENTS})
    endif()
endmacro()












macro(zhu_set_cxx_standard _version)
    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${_version} -stdlib=libc++")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++${_version}")
    endif()

    if ("${_version}" STREQUAL "17")
        zhu_compile_definitions(_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
        zhu_compile_definitions(_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
 
        zhu_compile_definitions(_HAS_AUTO_PTR_ETC)
        zhu_compile_definitions(_LIBCPP_ENABLE_CXX17_REMOVED_BINDERS)
        zhu_compile_definitions(_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR)
    endif()
endmacro()

macro(zhu_set_arch _arch)
    if ("${_arch}" STREQUAL "x64")
        if (ZHU_IS_64_BIT)
            if (ZHU_IS_TARGET_PLATFORM_WINDOWS)
                set(CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE "x64")
                target_compile_definitions(${__CurrentTargetName__} PRIVATE -D_WIN64)
            else()
                message(FATAL_ERROR "!!! TODO !!!")
            endif()
        else()
            message(FATAL_ERROR "!!! Can't build as 64bits on a 32bits machine !!!")
        endif()
    endif()
endmacro()

macro(zhu_enable_rtti)
    if(MSVC)
        target_compile_options(${__CurrentTargetName__} PRIVATE /GR)
    endif()
    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_compile_options(${__CurrentTargetName__} PRIVATE -frtti)
    endif()
endmacro()

macro(zhu_omit_default_libs)
    if(MSVC)
        target_compile_options(${__CurrentTargetName__} PRIVATE /Zl) #OmitDefaultLibName = true
    endif()
endmacro()

macro(zhu_set_charset _charset)
    if ("${_charset}" STREQUAL "UNICODE")
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -D_UNICODE)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -DUNICODE)
    endif()
endmacro()

macro(zhu_add_pch _h_file _cpp_file)
    if(MSVC)
        # Get the name for the .pch file
        get_filename_component(_precompiledBasename ${_h_file} NAME_WE)
        set(_temp "${CMAKE_CURRENT_BINARY_DIR}/${_precompiledBasename}.pch")
        file(TO_NATIVE_PATH "${_temp}" _pch_file)

        #use _pch_file in all sources
        target_compile_options(${_target} PRIVATE /Yu\"${_h_file}\" /Fp${_pch_file})
        #create pch file when building _cpp_file
        set_property(SOURCE ${_cpp_file} 
                     APPEND_STRING PROPERTY
                     COMPILE_FLAGS "/Yc\"${_h_file}\"")
    endif()

    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        # No easy way for general Clang. Currently, for Xcode only.
        if(ZHU_IS_TARGET_PLATFORM_OSX)
            if(NOT ZHU_USE_CCACHE)
                set_target_properties(${_target} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
            endif()
            set_target_properties(${_target} PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER ${_h_file})
        endif()
    endif()
endmacro()

macro(zhu_file_no_pch _cpp_file)
    if(MSVC)
        set_property(SOURCE ${_cpp_file} 
                     APPEND_STRING PROPERTY
                     COMPILE_FLAGS "/Y-")
    endif()
endmacro()

macro(zhu_set_target_folder _folder)
    set_target_properties(${__CurrentTargetName__} PROPERTIES FOLDER ${_folder})
endmacro()

macro(zhu_add_command _type _command)
    add_custom_command(TARGET ${__CurrentTargetName__} ${_type}
                       COMMAND ${_command})
endmacro()

macro(zhu_auto_group)
    foreach(_file ${ARGV})
        if (IS_ABSOLUTE "${_file}")
            file(RELATIVE_PATH _relativePath "${CMAKE_CURRENT_SOURCE_DIR}" "${_file}")
        else()
            set(_relativePath "${_file}")
        endif()

        if (${_relativePath} MATCHES "(.*\\.h$)|(.*\\.hpp$)")
            set(_prefix "Header Files")
        else()
            set(_prefix "Source Files")
        endif()
    
        get_filename_component(_path "${_relativePath}" PATH)
        string(REPLACE "/" "\\\\" _group "${_path}")
        source_group("${_prefix}\\\\${_group}" FILES "${_file}")
    endforeach()
endmacro()

macro(zhu_add_source_files)
    # Scan all directories
    set(_headerFileList)
    set(_sourceFileList)
    foreach(_fileEntry ${ARGV})
        set(_temp)
        get_filename_component(_absolutePath ${_fileEntry} ABSOLUTE)

        if(EXISTS ${_absolutePath})
            if(IS_DIRECTORY "${_absolutePath}")
                file(GLOB_RECURSE _temp "${_absolutePath}" "*.cpp" "*.c" "*.asm" "*.h")
            else()
                set(_temp "${_absolutePath}")
            endif()

            # Recognize .cpp/.h
            foreach(_item ${_temp})
                if (${_item} MATCHES "(.*\\.h$)|(.*\\.hpp$)")
                    list(APPEND _headerFileList ${_item})
                else()
                    list(APPEND _sourceFileList ${_item})
                endif()
            endforeach()
        endif()
    endforeach()

    # Add to __CurrentSourceFiles__
    set(_appendFiles ${_headerFileList} ${_sourceFileList})
    zhu_auto_group(${_appendFiles})
    list(APPEND __CurrentSourceFiles__ ${_appendFiles})
endmacro()













macro(zhu_add_shared_library _target)
    zhu_reset_current_target(${_target})
    add_library(${__CurrentTargetName__} SHARED "dummy.cpp")
    zhu_set_output_directories()
    zhu_load_target_version()
    message(STATUS "Generate shared library: " ${_target} "    " ${__CurrentTargetVersion__})
endmacro()

macro(zhu_add_static_library _target)
    zhu_reset_current_target(${_target})
    add_library(${__CurrentTargetName__} STATIC "dummy.cpp")
    zhu_set_output_directories()
    zhu_load_target_version()
    message(STATUS "Generate static library: " ${_target} "    " ${__CurrentTargetVersion__})
endmacro()

macro(zhu_add_executable _target)
    zhu_reset_current_target(${_target})
    if (ZHU_IS_TARGET_PLATFORM_IOS)
        add_executable(${__CurrentTargetName__} MACOSX_BUNDLE "dummy.cpp")
        set(MACOSX_BUNDLE_GUI_IDENTIFIER ${__CurrentTargetName__})
    else()
        add_executable(${__CurrentTargetName__} "dummy.cpp")
    endif()
    zhu_set_output_directories()
    zhu_load_target_version()
    message(STATUS "Generate executable app: " ${_target} "    " ${__CurrentTargetVersion__})
endmacro()

# "_type" can be SHARED, STATIC, INTERFACE
macro(zhu_add_imported_library _target _type)
    zhu_reset_current_target(${_target})
    set(__IsImportedTarget__ TRUE)
    add_library(${__CurrentTargetName__} IMPORTED ${_type} GLOBAL)
endmacro()












macro(zhu_end_shared_library)
    # Replace sources property with current source information    
    if (__CurrentSourceFiles__)
        set_target_properties(${__CurrentTargetName__} PROPERTIES SOURCES "${__CurrentSourceFiles__}")
    endif()

    zhu_apply_general_build_settings()
    zhu_apply_shared_library_build_settings()
    zhu_apply_link_flags()
endmacro()

macro(zhu_end_static_library)
    # Replace sources property with current source information    
    if (__CurrentSourceFiles__)
        set_target_properties(${__CurrentTargetName__} PROPERTIES SOURCES "${__CurrentSourceFiles__}")
    endif()

    zhu_apply_general_build_settings()
    zhu_apply_static_library_build_settings()
    zhu_apply_link_flags()
endmacro()

macro(zhu_end_executable)
    if (ZHU_IS_TARGET_PLATFORM_WINDOWS OR ZHU_IS_TARGET_PLATFORM_OSX)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${ENGINE_BIN_DIR}/${__CurTargetName__}")
        set_target_properties(${__CurrentTargetName__} PROPERTIES
                              RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ENGINE_BIN_DIR}/${__CurTargetName__}")
    endif()

    # Replace sources property with current source information    
    if (__CurrentSourceFiles__)
        set_target_properties(${__CurrentTargetName__} PROPERTIES SOURCES "${__CurrentSourceFiles__}")
    endif()

    zhu_apply_general_build_settings()
    zhu_apply_link_flags()

    if(ZHU_IS_TARGET_PLATFORM_IOS)
        set_target_properties(${__CurrentTargetName__} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
                              XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf"
                              XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER YES
                              XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS YES
                              XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
                              XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
                              XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC "YES"
                              FRAMEWORK TRUE)

        target_link_libraries(${__CurrentTargetName__} PUBLIC
                              # Commonly used UI related frameworks
                              "-framework UIKit"
                              "-framework QuartzCore"
                              "-framework CoreGraphics"
                              -ObjC)
    endif()
endmacro()

macro(zhu_end_imported_library)
    # Replace sources property with current source information    
    if(__CurrentSourceFiles__)
        message(FALTAL_ERROR "!!! There should be no source files for IMPORTED target !!!")
    endif()
    if(__CurrentLinkFlags__ OR __CurrentLinkFlagsDebug__ OR __CurrentLinkFlagsRelease__)
        message(FALTAL_ERROR "!!! There should be no link flags for IMPORTED target !!!")
    endif()
endmacro()
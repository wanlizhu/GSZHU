set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_DEBUG_POSTFIX "")

include(CMakeParseArguments)
include(GNUInstallDirs)

# One-time initialization of the build system. 
macro(cm_setup_project _proj)
    cmake_policy(SET CMP0079 NEW)    # allows the target_link_libraries() to run on targets created in other directories.

    if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
        set(CMAKE_BUILD_TYPE "Debug")
    endif()
    message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

    if(MSVC OR (CMAKE_GENERATOR STREQUAL "Xcode"))
        # disable subfolder("/Debug";"/Release") in output directory
        set(CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}")
        message(STATUS "CMAKE_CONFIGURATION_TYPES: ${CMAKE_CONFIGURATION_TYPES}")
    endif()

    set_property(GLOBAL PROPERTY USE_FOLDERS ON) # To allow making project folders in IDEs
    set(CMAKE_POSITION_INDEPENDENT_CODE      ON)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS     ON)

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

    message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY: " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    message(STATUS "")

    project(${_proj})

    # Should be called after project() when CMAKE_CXX_COMPILER_ID is set
    cm_setup_system_variables()
    enable_language(ASM_MASM)
endmacro()

# cm_reset_current_target(_target)
macro(cm_reset_current_target _target)
    set(__CurrentTargetName__ ${_target})
endmacro()

# cm_setup_output_directories()
# A runtime output artifact of a buildsystem target may be:
#   1. The executable file (e.g. .exe) of an executable target created by the add_executable() command.
#   2. On DLL platforms: the executable file (e.g. .dll) of a shared library target created by the add_library() command with the SHARED option.
macro(cm_setup_output_directories)
    set_target_properties(${__CurrentTargetName__} PROPERTIES 
        ARCHIVE_OUTPUT_DIRECTORY         ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY         ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        RUNTIME_OUTPUT_DIRECTORY         ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        PDB_OUTPUT_DIRECTORY             ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        PDB_OUTPUT_DIRECTORY_DEBUG       ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        PDB_OUTPUT_DIRECTORY_RELEASE     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endmacro()

# cm_disable_specific_warnings(item...)
macro(cm_disable_specific_warnings)
    foreach(_item ${ARGV})
        target_compile_options(${__CurrentTargetName__} PRIVATE /wd${_item})
    endforeach()
endmacro()

# cm_setup_compile_options()
macro(cm_setup_compile_options)
    if(IS_TARGET_PLATFORM_OSX)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -DCMAKE_TARGET_MAC)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -D_NATIVE_WCHAR_T_DEFINED)
    endif()

    if(IS_TARGET_PLATFORM_IOS)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -DCMAKE_TARGET_IOS -DCMAKE_TARGET_POSIX)
    endif()

    if(IS_TARGET_PLATFORM_ANDROID)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -DCMAKE_TARGET_ANDROID -DCMAKE_TARGET_POSIX)
    endif()

    if(IS_TARGET_PLATFORM_WINDOWS)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -DCMAKE_TARGET_WINDOWS)
    endif()

    if(IS_ISET_INTEL)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -DZHU_TARGET_INTEL)
    endif()
    
    if(IS_ISET_ARM)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -DZHU_TARGET_ARM)
    endif()

    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -D__cdecl=)
    endif()

    # compile definitions
    if(MSVC)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -DWIN32)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -D_WINDOWS)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT=1)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -D_ALLOW_RTCc_IN_STL)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -D_CRT_STDIO_ARBITRARY_WIDE_SPECIFIERS)
    endif()

    # compile options
    if (MSVC)
        target_compile_options(${__CurrentTargetName__} PRIVATE /openmp-)       # OpenMPSupport = false   
        target_compile_options(${__CurrentTargetName__} PRIVATE /fp:except-)    # FloatingPointExceptions = false
        target_compile_options(${__CurrentTargetName__} PRIVATE /GF)            # StringPooling = true
        target_compile_options(${__CurrentTargetName__} PRIVATE /Gy)            # FunctionLevelLinking = true
        target_compile_options(${__CurrentTargetName__} PRIVATE /Gm-)           # MinimalRebuild = false
        target_compile_options(${__CurrentTargetName__} PRIVATE /sdl)           # SDLCheck = true
        target_compile_options(${__CurrentTargetName__} PRIVATE /GS)            # BufferSecurityCheck = true
        target_compile_options(${__CurrentTargetName__} PRIVATE /MP)            # MultiProcessorCompilation = true
        if(NOT CMAKE_CL_64)
            target_compile_options(${__CurrentTargetName__} PRIVATE /arch:SSE2) # EnableEnhancedInstructionSet = StreamingSIMDExtensions2
        endif()

        # AdditionalOptions
        target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Debug>:/RTCc>)    # enable runtime check
        target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Release>:/GL>)    # enable whole program optimization for Release
        target_compile_options(${__CurrentTargetName__} PRIVATE /EHsc)                       # Enable C++ exceptions
        target_compile_options(${__CurrentTargetName__} PRIVATE /Qfast_transcendentals)
        target_compile_options(${__CurrentTargetName__} PRIVATE /Zc:inline)
        target_compile_options(${__CurrentTargetName__} PRIVATE /Zc:throwingNew)
        target_compile_options(${__CurrentTargetName__} PRIVATE /Zc:rvalueCast)
        target_compile_options(${__CurrentTargetName__} PRIVATE /bigobj)
        target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Debug>:/Od>)      # Optimization = Disabled
        target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Release>:/O2>)    # Optimization = MaxSpeed
        target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Debug>:/MDd>)     # RuntimeLibrary = MultiThreadedDebugDLL
        target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Release>:/MD>)    # RuntimeLibrary = MultiThreadedDLL
        target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Release>:/GT>)    # EnableFiberSafeOptimizations = true
        target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Release>:/Ot>)    # FavorSizeOrSpeed = Speed
        target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Release>:/Ob2>)   # InlineFunctionExpansion = AnySuitable
        target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Release>:/Oi>)    # IntrinsicFunctions = true
        target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Release>:/Oy>)    # OmitFramePointers = true
        target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Release>:/Zi>)    # DebugInformationFormat = ProgramDatabase
        if (MSVC_VERSION GREATER_EQUAL 1915)
            target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Debug>:/JMC>) # Just My Code Debugging = Enabled
        endif()
    endif()

    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_compile_options(${__CurrentTargetName__} PRIVATE -fexceptions)
        if (IS_TARGET_PLATFORM_OSX)
        	target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Debug>:-O0>)
            target_compile_options(${__CurrentTargetName__} PRIVATE $<$<CONFIG:Release>:-Os>)
        endif()

        target_compile_options(${__CurrentTargetName__} 
            PRIVATE -Wno-backslash-newline-escape
            PRIVATE -Wno-write-strings
            PRIVATE -Wno-unused-const-variable
            PRIVATE -Wno-unused-variable
            PRIVATE -Wno-return-type-c-linkage
            PRIVATE -Wno-deprecated-register
            PRIVATE -Wno-unknown-warning-option
            PRIVATE -Wno-new-returns-null
            PRIVATE -Wno-extra-tokens
            PRIVATE -Wno-parentheses
            PRIVATE -Wno-comment
            PRIVATE -Wno-switch-enum
            PRIVATE -Wno-switch
            PRIVATE -Wno-delete-non-virtual-dtor
            PRIVATE -Wno-deprecated-declarations
            PRIVATE -Wno-dangling-else
            PRIVATE -Wno-vexing-parse
            PRIVATE -Wno-reserved-user-defined-literal
            PRIVATE -Wno-undefined-bool-conversion
            PRIVATE -Wno-tautological-undefined-compare
            PRIVATE -Wno-invalid-offsetof
            PRIVATE -Wno-nonportable-include-path # since Xcode 8.3: Ignores case sensitivity include issues - there are more than a thousand issues like this, in case we decide to port to an OS where file case sensitivity matter (e.g Linux) we should remove this.
            PRIVATE -Wno-nonportable-system-include-path
            PRIVATE -Wno-undefined-var-template # This appeared since Xcode 8.3
            PRIVATE -Wreturn-type # ensure every non-void method/function has a return statement
            PRIVATE -Wno-null-conversion
            PRIVATE -Wno-null-arithmetic
            PRIVATE -Wno-tautological-constant-out-of-range-compare
            PRIVATE -Wno-c++11-narrowing
            PRIVATE -Wno-tautological-constant-compare
            PRIVATE -Wno-enum-compare-switch # For patched 1.38.4
            PRIVATE -Wno-pragma-pack # For patched 1.38.4
            PRIVATE -Wno-ignored-pragmas)
    endif()
endmacro()

# cm_setup_link_options()
macro(cm_setup_link_options)
    # Make sure we append additional flags to LINK_FLAGS property
    get_target_property(_currentLinkFlags  ${__CurrentTargetName__} LINK_FLAGS)
    get_target_property(_currentLinkFlagsD ${__CurrentTargetName__} LINK_FLAGS_DEBUG)
    get_target_property(_currentLinkFlagsR ${__CurrentTargetName__} LINK_FLAGS_RELEASE)

    if("${_currentLinkFlags}" STREQUAL "_currentLinkFlags-NOTFOUND")
        set(_currentLinkFlags "")
    endif()
    if("${_currentLinkFlagsD}" STREQUAL "_currentLinkFlagsD-NOTFOUND")
        set(_currentLinkFlagsD "")
    endif()
    if("${_currentLinkFlagsR}" STREQUAL "_currentLinkFlagsR-NOTFOUND")
        set(_currentLinkFlagsR "")
    endif()

     set_target_properties(${__CurrentTargetName__} PROPERTIES LINK_FLAGS "${_currentLinkFlags}")
     set_target_properties(${__CurrentTargetName__} PROPERTIES LINK_FLAGS_DEBUG "${_currentLinkFlagsD}")
     set_target_properties(${__CurrentTargetName__} PROPERTIES LINK_FLAGS_RELEASE "${_currentLinkFlagsR}")
endmacro()

# cm_setup_install_options()
macro(cm_setup_install_options)
    install(TARGETS ${__CurrentTargetName__} 
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

    if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include")
        install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include" DESTINATION include)
    endif()
endmacro()

# This macro should be called after project() command.
# Otherwise, CMAKE_SYSTEM_NAME and CMAKE_HOST_SYSTEM_NAME not initialized.
macro(cm_setup_system_variables)
    if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set(IS_TARGET_PLATFORM_WINDOWS TRUE)
    else()
        set(IS_TARGET_PLATFORM_WINDOWS FALSE)
    endif()

    string(TOLOWER "${CMAKE_OSX_SYSROOT}" _OSX_SYSROOT_LOWER)
    if((${CMAKE_SYSTEM_NAME} MATCHES "Darwin") AND 
       ("${_OSX_SYSROOT_LOWER}" MATCHES "macosx"))
        set(IS_TARGET_PLATFORM_OSX TRUE)
    else()
        set(IS_TARGET_PLATFORM_OSX FALSE)
    endif()

    if (IS_TARGET_PLATFORM_WINDOWS OR IS_TARGET_PLATFORM_OSX)
      set(IS_TARGET_PLATFORM_DESKTOP TRUE)
    endif()

    if((${CMAKE_SYSTEM_NAME} MATCHES "Darwin") AND
       ("${_OSX_SYSROOT_LOWER}" MATCHES "iphone"))
        set(IS_TARGET_PLATFORM_IOS TRUE)
        if("${_OSX_SYSROOT_LOWER}" MATCHES "iphoneos")
            set(IS_TARGET_PLATFORM_IOS_DEVICE TRUE)
        endif()
        if("${_OSX_SYSROOT_LOWER}" MATCHES "iphonesimulator")
            set(IS_TARGET_PLATFORM_IOS_SIMULATOR TRUE)
        endif()
    else()
        set(IS_TARGET_PLATFORM_IOS FALSE)
    endif()

    if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
        set(IS_TARGET_PLATFORM_ANDROID TRUE)
    else()
        set(IS_TARGET_PLATFORM_ANDROID FALSE)
    endif()

    if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        set(IS_BUILD_MACHINE_WINDOWS TRUE)
    else()
        set(IS_BUILD_MACHINE_WINDOWS FALSE)
    endif()

    if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
        set(IS_BUILD_MACHINE_OSX TRUE)
    else()
        set(IS_BUILD_MACHINE_OSX FALSE)
    endif()

    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        set(IS_32BIT_ARCH TRUE)
        set(IS_64BIT_ARCH FALSE)
    else()
        set(IS_32BIT_ARCH FALSE)
        set(IS_64BIT_ARCH TRUE)
    endif()

    if(IS_TARGET_PLATFORM_IOS_DEVICE        OR
       CMAKE_VS_PLATFORM_NAME MATCHES "ARM" OR
       CMAKE_ANDROID_ARCH_ABI MATCHES "arm")
        # ISET == "Instruction Set"
        set(IS_TARGET_ISET_ARM TRUE)
        set(IS_TARGET_ISET_INTEL FALSE)
    else()
        set(IS_TARGET_ISET_ARM FALSE)
        set(IS_TARGET_ISET_INTEL TRUE)
    endif()
endmacro()

macro(cm_load_target_version)
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

    set(_version "${_major}.${_minor}.${_patch}")
    set_target_properties(${__CurrentTargetName__} PROPERTIES VERSION ${_version})
endmacro()

macro(cm_treat_warnings_as_errors)
    if(MSVC)
        target_compile_options(${__CurrentTargetName__} PRIVATE /WX)
    endif()

    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_compile_options(${__CurrentTargetName__} PRIVATE -Werror)
        # CMake hard coded "-Wmost" for Xcode which will generate more warnings than main stream build.
        # So, we disable "-Wmost" for the moment. Long term solution is to fix the code so that no warning generated.
        target_compile_options(${__CurrentTargetName__} PRIVATE -Wno-most)
    endif()
endmacro()

# cm_set_output_name(Name [DEBUG NameD] [RELEASE NameR])
macro(cm_set_output_name)
    set(_oneValueArgs DEBUG RELEASE)
    cmake_parse_arguments(_ARGS "" "${_oneValueArgs}" "" ${ARGN})

    string(COMPARE EQUAL "${_ARGS_DEBUG}"              "" _emptyNameD)
    string(COMPARE EQUAL "${_ARGS_RELEASE}"            "" _emptyNameR)
    string(COMPARE EQUAL "${_ARGS_UNPARSED_ARGUMENTS}" "" _emptyName)
    
    if (NOT _emptyName)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              OUTPUT_NAME "${_ARGS_UNPARSED_ARGUMENTS}")
    endif()
    if (NOT _emptyNameD)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              OUTPUT_NAME_DEBUG "${_ARGS_DEBUG}")
    endif()
    if (NOT _emptyNameR)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              OUTPUT_NAME_RELEASE "${_ARGS_RELEASE}")
    endif()
endmacro()

# cm_add_resources(res... [FOLDER folder])
# if FOLDER is specified, folder will be created in output directory, to which res.. will be copied 
# only executable targets can add resources to
macro(cm_add_resources)
    if(__IsImportedTarget__)
        message(FATAL_ERROR "!!! IMPORTED target is not allowed for cm_add_resources() !!!")
    endif()
    
    set(_oneValueArgs PUBLIC INTERFACE)
    cmake_parse_arguments(_ARGS "" "${_oneValueArgs}" "" ${ARGN})

    get_target_property(_outputDirectory ${__CurrentTargetName__} RUNTIME_OUTPUT_DIRECTORY)
    if ("${_ARGS_FOLDER}" STREQUAL "")
        set(_dstFolder "${_outputDirectory}")
    else()
        set(_dstFolder "${_outputDirectory}/${_ARGS_FOLDER}")
    endif()

    set(_filesToCopy)
    set(_dirsToCopy)

    foreach(_res ${_ARGS_UNPARSED_ARGUMENTS})
        if(EXISTS ${_res})
            if(IS_DIRECTORY ${_res})
                list(APPEND _dirsToCopy ${_res})
            else()
                list(APPEND _filesToCopy ${_res})
            endif()
        endif()
    endforeach()

    if(_filesToCopy)
        add_custom_command(TARGET ${__CurrentTargetName__} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_files ${_filesToCopy} ${_dstFolder})
    endif()
    if(_dirsToCopy)
        add_custom_command(TARGET ${__CurrentTargetName__} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_directory ${_dirsToCopy} ${_dstFolder})
    endif()
endmacro()

# cm_compile_definitions(def... [PUBLIC def...] [INTERFACE def...])
# Arguments to target_compile_definitions may use "generator expressions"
# Any leading -D on an item will be removed. Empty items are ignored
# PRIVATE and PUBLIC items will populate the COMPILE_DEFINITIONS property of <target>
# PUBLIC  and INTERFACE items will populate the INTERFACE_COMPILE_DEFINITIONS property of <target>
# IMPORTED targets only support INTERFACE items
# PRIVATE is the default scope
macro(cm_compile_definitions)
    set(_multiValueArgs PUBLIC INTERFACE)
    cmake_parse_arguments(_ARGS "" "" "${_multiValueArgs}" ${ARGN})

    foreach(_def ${_ARGS_UNPARSED_ARGUMENTS})
        target_compile_definitions(${__CurrentTargetName__} PRIVATE ${_def})
    endforeach()
    foreach(_def ${_ARGS_PUBLIC})
        target_compile_definitions(${__CurrentTargetName__} PUBLIC ${_def})
    endforeach()
    foreach(_def ${_ARGS_INTERFACE})
        target_compile_definitions(${__CurrentTargetName__} INTERFACE ${_def})
    endforeach()
endmacro()

# cm_include_directories(inc... [PUBLIC inc...] [INTERFACE inc...])
# Arguments to target_include_directories may use "generator expressions"
# PRIVATE and PUBLIC items will populate the INCLUDE_DIRECTORIES property of <target>
# PUBLIC and INTERFACE items will populate the INTERFACE_INCLUDE_DIRECTORIES property of <target>
# IMPORTED targets only support INTERFACE items 
# PRIVATE is the default scope
macro(cm_include_directories)
    set(_multiValueArgs PUBLIC INTERFACE)
    cmake_parse_arguments(_ARGS "" "" "${_multiValueArgs}" ${ARGN})

    foreach(_inc ${_ARGS_UNPARSED_ARGUMENTS})
        target_include_directories(${__CurrentTargetName__} PRIVATE ${_inc})
    endforeach()
    foreach(_inc ${_ARGS_PUBLIC})
        target_include_directories(${__CurrentTargetName__} PUBLIC ${_inc})
    endforeach()
    foreach(_inc ${_ARGS_INTERFACE})
        target_include_directories(${__CurrentTargetName__} INTERFACE ${_inc})
    endforeach()
endmacro()

# cm_additional_library_directories(dir... [PUBLIC dir...] [INTERFACE dir...])
# PRIVATE and PUBLIC items will populate the LINK_DIRECTORIES property of <target> 
# PUBLIC  and INTERFACE items will populate the INTERFACE_LINK_DIRECTORIES property of <target> 
# IMPORTED targets only support INTERFACE items
macro(cm_additional_library_directories)
    set(_multiValueArgs PUBLIC INTERFACE)
    cmake_parse_arguments(_ARGS "" "" "${_multiValueArgs}" ${ARGN})

    foreach(_inc ${_ARGS_UNPARSED_ARGUMENTS})
        target_link_directories(${__CurrentTargetName__} BEFORE PRIVATE ${_inc})
    endforeach()
    foreach(_inc ${_ARGS_PUBLIC})
        target_link_directories(${__CurrentTargetName__} BEFORE PUBLIC ${_inc})
    endforeach()
    foreach(_inc ${_ARGS_INTERFACE})
        target_link_directories(${__CurrentTargetName__} BEFORE INTERFACE ${_inc})
    endforeach()
endmacro()

# cm_link_libraries([PUBLIC item...] [INTERFACE item...] ...)
# Each <item> may be:
#     A library target name
#     A full path to a library file
#     A plain library name
#     A generator expression
#
# PUBLIC: will be linked to, and are made part of the link interface. 
# PRIVATE: will be linked to, but are not made part of the link interface. 
# INTERFACE: will be appended to the link interface and are NOT used for linking <target>.
# the default mode is PRIVATE
macro(cm_link_libraries)
    set(_multiValueArgs PUBLIC INTERFACE)
    cmake_parse_arguments(_ARGS "" "" "${_multiValueArgs}" ${ARGN})

    foreach(_lib ${_ARGS_UNPARSED_ARGUMENTS})
        target_link_libraries(${__CurrentTargetName__} PRIVATE ${_lib})
        cm_copy_linked_binaries(${_lib})
    endforeach()
    foreach(_lib ${_ARGS_PUBLIC})
        target_link_libraries(${__CurrentTargetName__} PUBLIC ${_lib})
        cm_copy_linked_binaries(${_lib})
    endforeach()
    foreach(_lib ${_ARGS_INTERFACE})
        target_link_libraries(${__CurrentTargetName__} INTERFACE ${_lib})
        cm_copy_linked_binaries(${_lib})
    endforeach()
endmacro()

# cm_is_imported_target(_target _result _locationD _locationR)
# _result is true if _target is an imported target
# _locationD is the IMPORTED_LOCATION_DEBUG property of _target
# _locationR is the IMPORTED_LOCATION_RELEASE property of _target
macro(cm_get_imported_location _target _isImported _locationD _locationR)
    set(${_isImported} FALSE)
    set(${_locationD})
    set(${_locationR})

    get_target_property(_targetType ${_target} TYPE)
    if (NOT "${_targetType}" STREQUAL "INTERFACE_LIBRARY")
        get_target_property(${_locationD} ${_target} IMPORTED_LOCATION_DEBUG)
        get_target_property(${_locationR} ${_target} IMPORTED_LOCATION_RELEASE)

        if ((NOT ${_locationD} MATCHES ".*NOTFOUND$") OR
            (NOT ${_locationR} MATCHES ".*NOTFOUND$"))
            set(${_isImported} TRUE)
        endif()
    endif()
endmacro()

# cm_copy_linked_binaries(lib...)
macro(cm_copy_linked_binaries)
    foreach(_item ${ARGV})
        set(_targetType)
        set(_isImported)
        set(_locationD)
        set(_locationR)
        set(_outpathD)
        set(_outpathR)

        get_target_property(_targetType ${_item} TYPE)
        if ((NOT "${_targetType}" STREQUAL "INTERFACE_LIBRARY") AND
            (NOT "${_targetType}" STREQUAL "STATIC_LIBRARY"))

            cm_get_imported_location(${_item} _isImported _locationD _locationR)
 
            if (${_isImported})
                cm_post_build_copy(DEBUG "${_locationD}" RELEASE "${_locationR}")
            else()
                get_target_property(_outdirD  ${_item} RUNTIME_OUTPUT_DIRECTORY_DEBUG)
                get_target_property(_outnameD ${_item} OUTPUT_NAME_DEBUG)
                get_filename_component(_outpathD "${_outnameD}" ABSOLUTE  BASE_DIR "${_outdirD}")

                get_target_property(_outdirR  ${_item} RUNTIME_OUTPUT_DIRECTORY_RELEASE)
                get_target_property(_outnameR ${_item} OUTPUT_NAME_RELEASE)
                get_filename_component(_outpathR "${_outnameR}" ABSOLUTE  BASE_DIR "${_outdirR}")

                cm_post_build_copy(DEBUG "${_outpathD}" RELEASE "${_outpathR}")
            endif()
        endif()
    endforeach()
endmacro()

# cm_post_build_copy([DEBUG file...] [RELEASE file...])
macro(cm_post_build_copy)
    set(_multiValueArgs DEBUG RELEASE)
    cmake_parse_arguments(_ARGS "" "" "${_multiValueArgs}" ${ARGN})

    # get destination directory
    get_target_property(_outdirD ${__CurrentTargetName__} RUNTIME_OUTPUT_DIRECTORY_DEBUG)
    get_target_property(_outdirR ${__CurrentTargetName__} RUNTIME_OUTPUT_DIRECTORY_RELEASE)

    # collect files to copy according to CMAKE_BUILD_TYPE
    set(_filesToCopy "${_ARGS_UNPARSED_ARGUMENTS}")
    string(TOLOWER "${CMAKE_BUILD_TYPE}" _CMAKE_BUILD_TYPE)
    if ("${_CMAKE_BUILD_TYPE}" STREQUAL "debug")
        set(_filesToCopy "${_filesToCopy}" "${_ARGS_DEBUG}")
        set(_dstdir "${_outdirD}")
    elseif("${_CMAKE_BUILD_TYPE}" STREQUAL "release")
        set(_filesToCopy "${_filesToCopy}" "${_ARGS_RELEASE}")
        set(_dstdir "${_outdirR}")
    else()
        message(FATAL_ERROR "!!! CMAKE_BUILD_TYPE must be either Debug or Release !!!")
    endif()

    

    # remove from files-to-copy if it already at the destination directory
    set(_filesToCopySanitized)
    foreach(_file ${_filesToCopy})
        get_filename_component(_path ${_file} ABSOLUTE)
        get_filename_component(_path ${_path} DIRECTORY)
        if (NOT "${_path}" STREQUAL "${_dstdir}")
            list(APPEND _filesToCopySanitized "${_file}")
        endif()
    endforeach()

    #message("Files to copy: " ${_filesToCopySanitized})
    
    # final step, make the copy command
    if (NOT "${_filesToCopySanitized}" STREQUAL "")
        cm_add_command(POST_BUILD "${CMAKE_COMMAND}" -E copy "${_filesToCopySanitized}" "${_dstdir}")
    endif()
endmacro()

# cm_set_imported_location(loc [DEBUG locD] [RELEASE locR])
macro(cm_set_imported_location)
    set(_oneValueArgs DEBUG RELEASE)
    cmake_parse_arguments(_ARGS "" "${_oneValueArgs}" "" ${ARGN})

    if(_ARGS_DEBUG)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              IMPORTED_LOCATION_DEBUG ${_ARGS_DEBUG})
    endif()
    if(_ARGS_RELEASE)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              IMPORTED_LOCATION_RELEASE ${_ARGS_RELEASE})
    endif()
    if(_ARGS_UNPARSED_ARGUMENTS)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              IMPORTED_LOCATION ${_ARGS_UNPARSED_ARGUMENTS})
    endif()
endmacro()

# cm_set_imported_implib(loc [DEBUG locD] [RELEASE locR])
macro(cm_set_imported_implib)
    set(_oneValueArgs DEBUG RELEASE)
    cmake_parse_arguments(_ARGS "" "${_oneValueArgs}" "" ${ARGN})

    if(_ARGS_DEBUG)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              IMPORTED_IMPLIB_DEBUG ${_ARGS_DEBUG})
    endif()
    if(_ARGS_RELEASE)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              IMPORTED_IMPLIB_RELEASE ${_ARGS_RELEASE})
    endif()
    if(_ARGS_UNPARSED_ARGUMENTS)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              IMPORTED_IMPLIB ${_ARGS_UNPARSED_ARGUMENTS})
    endif()
endmacro()

# cm_set_cxx_standard(<11,14,17>)
macro(cm_set_cxx_standard _version)
    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${_version} -stdlib=libc++")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++${_version}")
    endif()
endmacro()

# cm_set_arch(<x86,x64>)
macro(cm_set_arch _arch)
    if ("${_arch}" STREQUAL "x64")
        if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
            if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
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

# cm_enable_rtti()
macro(cm_enable_rtti)
    if(MSVC)
        target_compile_options(${__CurrentTargetName__} PRIVATE /GR)
    endif()
    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_compile_options(${__CurrentTargetName__} PRIVATE -frtti)
    endif()
endmacro()

# cm_omit_default_libs()
macro(cm_omit_default_libs)
    if(MSVC)
        target_compile_options(${__CurrentTargetName__} PRIVATE /Zl) #OmitDefaultLibName = true
    endif()
endmacro()

# cm_set_charset(UNICODE)
macro(cm_set_charset _charset)
    if ("${_charset}" STREQUAL "UNICODE")
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -D_UNICODE)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE -DUNICODE)
    endif()
endmacro()

# cm_add_pch(HEADER header SOURCE src)
macro(cm_add_pch _header _source)
    set(_oneValueArgs HEADER SOURCE)
    cmake_parse_arguments(_ARGS "" "${_oneValueArgs}" "" ${ARGN})

    if(MSVC)
        # Get the name for the .pch file
        get_filename_component(_precompiledBasename ${_ARGS_HEADER} NAME_WE)
        set(_temp "${CMAKE_CURRENT_BINARY_DIR}/${_precompiledBasename}.pch")
        file(TO_NATIVE_PATH "${_temp}" _generatedPCHFile)

        #use _generatedPCHFile in all sources
        target_compile_options(${__CurrentTargetName__} PRIVATE /Yu\"${_ARGS_HEADER}\" /Fp${_generatedPCHFile})
        
        #create pch file when building _ARGS_SOUORCE
        set_property(SOURCE ${_ARGS_SOUORCE} 
                     APPEND_STRING PROPERTY
                     COMPILE_FLAGS "/Yc\"${_ARGS_HEADER}\"")
    endif()

    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        # No easy way for general Clang. Currently, for Xcode only.
        if(IS_TARGET_PLATFORM_OSX)
            set_target_properties(${_target} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
            set_target_properties(${_target} PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER ${_ARGS_HEADER})
        endif()
    endif()
endmacro()

# cm_file_no_pch(_file)
macro(cm_file_no_pch _file)
    if(MSVC)
        set_property(SOURCE ${_file} 
                     APPEND_STRING PROPERTY
                     COMPILE_FLAGS "/Y-")
    endif()
endmacro()

# cm_set_target_folder(_folder)
macro(cm_set_target_folder _folder)
    set_target_properties(${__CurrentTargetName__} PROPERTIES FOLDER ${_folder})
endmacro()

# cm_add_command(<PRE_BUILD|PRE_LINK|POST_BUILD> ...)
macro(cm_add_command _type)
    add_custom_command(TARGET ${__CurrentTargetName__} ${_type}
                       COMMAND ${ARGN})
endmacro()

# cm_use_package([REQUIRED item...] [OPTIONAL item...])
# CMAKE_item_NOTFOUND will be added to compile definitions if CMake cannot find it
# CMAKE_item_FOUND    will be added to compile definitions if CMake does find it
macro(cm_use_package)
    set(_multiValueArgs "REQUIRED" "OPTIONAL")
    cmake_parse_arguments(_ARGS "" "" "${_multiValueArgs}" ${ARGN})

    foreach(_package ${_ARGS_REQUIRED})
        find_package(${_package} REQUIRED)
        set(_includes  ${_package}_INCLUDE_DIRS)
        set(_libraries ${_package}_LIBRARIES)
        target_include_directories(${__CurrentTargetName__} PRIVATE ${_includes})
        target_link_libraries(${__CurrentTargetName__}      PRIVATE ${_libraries})
        string(TOUPPER ${_package} _PACKAGE)
        target_compile_definitions(${__CurrentTargetName__} PRIVATE "CMAKE_${_PACKAGE}_FOUND")
    endforeach()
    foreach(_package ${_ARGS_OPTIONAL})
        find_package(${_package})
        if (${_package}_FOUND)
            set(_includes  ${_package}_INCLUDE_DIRS)
            set(_libraries ${_package}_LIBRARIES)
            target_include_directories(${__CurrentTargetName__} PRIVATE ${_includes})
            target_link_libraries(${__CurrentTargetName__}      PRIVATE ${_libraries})
            string(TOUPPER ${_package} _PACKAGE)
            target_compile_definitions(${__CurrentTargetName__} PRIVATE "CMAKE_${_PACKAGE}_FOUND")
        else()
            string(TOUPPER ${_package} _PACKAGE)
            target_compile_definitions(${__CurrentTargetName__} PRIVATE "CMAKE_${_PACKAGE}_NOTFOUND")
        endif()
    endforeach()
endmacro()

# cm_auto_group(file...)
# Add files to certain groups according to their relative paths
macro(cm_auto_group)
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

# cm_add_source_files(<dir|file>...)
# for "directory" item, scan this directory recursively for all files within
macro(cm_add_source_files)
    # Scan all directories
    set(_headerFileList)
    set(_sourceFileList)
    foreach(_fileEntry ${ARGV})
        set(_temp)
        get_filename_component(_absolutePath ${_fileEntry} ABSOLUTE)

        if(EXISTS ${_absolutePath})
            if(IS_DIRECTORY "${_absolutePath}")
                file(GLOB_RECURSE _temp "${_absolutePath}" "*.cpp" "*.c" "*.asm" "*.h" "*.hpp")
            else()
                set(_temp "${_absolutePath}")
            endif()

            # recognize .cpp/.h
            foreach(_item ${_temp})
                if (${_item} MATCHES "(.*\\.h$)|(.*\\.hpp$)")
                    list(APPEND _headerFileList ${_item})
                else()
                    list(APPEND _sourceFileList ${_item})
                endif()
            endforeach()
        endif()
    endforeach()

    # group source files
    set(_newFiles ${_headerFileList} ${_sourceFileList})
    cm_auto_group(${_newFiles})
   
    if (_newFiles)
        # get current source list
        get_target_property(_currentTargetSources ${__CurrentTargetName__} SOURCES)
        if ("${_currentTargetSources}" STREQUAL "dummy.cpp")
            set(_currentTargetSources)
        endif()

        # add to target's SOURCES property
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              SOURCES "${_newFiles}")
    endif()
endmacro()

# cm_add_shared_library(_target)
macro(cm_add_shared_library _target)
    add_library(${_target} SHARED "dummy.cpp")
    cm_reset_current_target(${_target})
    cm_load_target_version()
    cm_setup_output_directories()
    cm_setup_compile_options()
    cm_setup_link_options()
    message(STATUS "Generate shared library: " ${_target} "\t" ${__CurrentTargetVersion__})
endmacro()

# cm_add_static_library(_target)
macro(cm_add_static_library _target)
    add_library(${__CurrentTargetName__} STATIC "dummy.cpp")
    cm_reset_current_target(${_target})
    cm_load_target_version()
    cm_setup_output_directories()
    cm_setup_compile_options()
    cm_setup_link_options()
    message(STATUS "Generate static library: " ${_target} "\t" ${__CurrentTargetVersion__})
endmacro()

# cm_add_executable(_target)
macro(cm_add_executable _target)
    add_executable(${_target} "dummy.cpp")
    cm_reset_current_target(${_target})
    cm_load_target_version()
    cm_setup_output_directories()
    cm_setup_compile_options()
    cm_setup_link_options()
    message(STATUS "Generate executable app: " ${_target} "\t" ${__CurrentTargetVersion__})
endmacro()

# cm_add_imported_library(_target <SHARED|STATIC|INTERFACE>)
macro(cm_add_imported_library)
    set(_options "SHARED" "STATIC" "INTERFACE")
    cmake_parse_arguments(_ARGS "${_options}" "" "" ${ARGN})

    if (_ARGS_INTERFACE)
        add_library(${_ARGS_UNPARSED_ARGUMENTS} IMPORTED INTERFACE GLOBAL)
    endif()
    if (_ARGS_STATIC)
        add_library(${_ARGS_UNPARSED_ARGUMENTS} IMPORTED STATIC GLOBAL)
    endif()
    if (_ARGS_SHARED)
        add_library(${_ARGS_UNPARSED_ARGUMENTS} IMPORTED SHARED GLOBAL)
    endif()

    cm_reset_current_target(${_ARGS_UNPARSED_ARGUMENTS})
    set(__IsImportedTarget__ TRUE)
endmacro()

# cm_end_shared_library()
macro(cm_end_shared_library)
endmacro()

# cm_end_static_library()
macro(cm_end_static_library)
endmacro()

# cm_end_executable()
macro(cm_end_executable)
endmacro()

# cm_end_imported_library()
macro(cm_end_imported_library)
endmacro()

# cm_use_package_glfw3([STATIC|SHARED])
# link against SHARED library by default
macro(cm_use_package_glfw3)
    set(_options "SHARED" "STATIC")
    cmake_parse_arguments(_ARGS "${_options}" "" "" ${ARGN})

    if (_ARGS_STATIC)
        find_package(glfw3 CONFIG REQUIRED)
        target_link_libraries(${__CurrentTargetName__} PRIVATE glfw)
    else()

    endif()
endmacro()
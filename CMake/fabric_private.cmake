# This macro should be called after project() command.
# Otherwise, CMAKE_SYSTEM_NAME and CMAKE_HOST_SYSTEM_NAME not initialized.
macro(zhu_set_system_variables)
    set(ENGINE_BIN_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    set(ENGINE_LIB_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

    if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set(ZHU_IS_TARGET_PLATFORM_WINDOWS TRUE)
    else()
        set(ZHU_IS_TARGET_PLATFORM_WINDOWS FALSE)
    endif()

    string(TOLOWER "${CMAKE_OSX_SYSROOT}" _OSX_SYSROOT_LOWER)

    if((${CMAKE_SYSTEM_NAME} MATCHES "Darwin") AND 
       ("${_OSX_SYSROOT_LOWER}" MATCHES "macosx"))
        set(ZHU_IS_TARGET_PLATFORM_OSX TRUE)
    else()
        set(ZHU_IS_TARGET_PLATFORM_OSX FALSE)
    endif()

    if (ZHU_IS_TARGET_PLATFORM_WINDOWS OR ZHU_IS_TARGET_PLATFORM_OSX)
      set(ZHU_IS_TARGET_PLATFORM_DESKTOP TRUE)
    endif()

    if((${CMAKE_SYSTEM_NAME} MATCHES "Darwin") AND
       ("${_OSX_SYSROOT_LOWER}" MATCHES "iphone"))
        set(ZHU_IS_TARGET_PLATFORM_IOS TRUE)
        if("${_OSX_SYSROOT_LOWER}" MATCHES "iphoneos")
            set(ZHU_IS_TARGET_PLATFORM_IOS_DEVICE TRUE)
        endif()
        if("${_OSX_SYSROOT_LOWER}" MATCHES "iphonesimulator")
            set(ZHU_IS_TARGET_PLATFORM_IOS_SIMULATOR TRUE)
        endif()
    else()
        set(ZHU_IS_TARGET_PLATFORM_IOS FALSE)
    endif()

    if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
        set(ZHU_IS_TARGET_PLATFORM_ANDROID TRUE)
    else()
        set(ZHU_IS_TARGET_PLATFORM_ANDROID FALSE)
    endif()

    if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        set(ZHU_IS_BUILD_MACHINE_WINDOWS TRUE)
    else()
        set(ZHU_IS_BUILD_MACHINE_WINDOWS FALSE)
    endif()

    if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
        set(ZHU_IS_BUILD_MACHINE_OSX TRUE)
    else()
        set(ZHU_IS_BUILD_MACHINE_OSX FALSE)
    endif()

    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "4")
        if(NOT DEFINED ZHU_IS_32_BIT)
            set(ZHU_IS_32_BIT TRUE)
        endif()
        if(NOT DEFINED ZHU_IS_64_BIT)
            set(ZHU_IS_64_BIT FALSE)
        endif()
    else()
        if(NOT DEFINED ZHU_IS_32_BIT)
            set(ZHU_IS_32_BIT FALSE)
        endif()
        if(NOT DEFINED ZHU_IS_64_BIT)
            set(ZHU_IS_64_BIT TRUE)
        endif()
    endif()

    if(ZHU_IS_TARGET_PLATFORM_IOS_DEVICE    OR
       CMAKE_VS_PLATFORM_NAME MATCHES "ARM" OR
       CMAKE_ANDROID_ARCH_ABI MATCHES "arm")
        set(ZHU_IS_TARGET_ARM TRUE)
        set(ZHU_IS_TARGET_INTEL FALSE)
    else()
        set(ZHU_IS_TARGET_ARM FALSE)
        set(ZHU_IS_TARGET_INTEL TRUE)
    endif()

    if (ZHU_IS_TARGET_PLATFORM_IOS)
        if ("${METAL}" EQUAL "1")
            set(ZHU_ENABLE_METAL TRUE)
        endif()

        # iOS output directories are overrided by xctools somehow, so CMake doesn't know target location.
        # Specify output directory explicitly here to let CMake esepcially CPack know the location
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug-${CMAKE_OSX_SYSROOT}")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug-${CMAKE_OSX_SYSROOT}")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug-${CMAKE_OSX_SYSROOT}")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release-${CMAKE_OSX_SYSROOT}")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release-${CMAKE_OSX_SYSROOT}")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release-${CMAKE_OSX_SYSROOT}")
    endif()

endmacro()

macro(zhu_hide_inlines)
    if(ZHU_IS_TARGET_PLATFORM_OSX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden")
    endif()
endmacro()

macro(zhu_reset_current_target _target)
    set(__IsImportedTarget__ FALSE)
    set(__CurrentDirectory__ ${CMAKE_CURRENT_SOURCE_DIR})
    set(__CurrentGroup__)
    set(__CurrentSourceFiles__)
    set(__CurrentResources__)

    set(__CurrentLinkFlags__)
    set(__CurrentLinkFlagsRelease__)
    set(__CurrentLinkFlagsDebug__)

    set(__CurrentTargetName__ ${_target})
    set(__CurrentTargetOutputName__ ${_target})
    set(__CurrentTargetVersion__ "1.0.0")
    set(__CurrentTargetVersionMajor__ 1)
    set(__CurrentTargetVersionMinor__ 0)
    set(__CurrentTargetVersionPatch__ 0)
endmacro()

macro(zhu_set_output_directories)
    set_target_properties(${__CurrentTargetName__} PROPERTIES 
                          ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
                          ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
                          LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
                          LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
                          RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                          RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                          PDB_OUTPUT_DIRECTORY_DEBUG       ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                          PDB_OUTPUT_DIRECTORY_RELEASE     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endmacro()

macro(zhu_apply_general_build_settings)
    string(COMPARE EQUAL "${__CurrentTargetOutputName__}" "" _emptyName)
    if (NOT _emptyName)
        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              OUTPUT_NAME "${__CurrentTargetOutputName__}")
    endif()

    zhu_general_compile_options(${__CurrentTargetName__})
    zhu_general_preprocessor_definitions(${__CurrentTargetName__})
    zhu_general_link_options(${__CurrentTargetName__})
    zhu_general_disable_specific_warnings(${__CurrentTargetName__})

    install(TARGETS ${__CurrentTargetName__} 
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
    if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include")
        install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include" DESTINATION include)
    endif()
endmacro()

macro(zhu_apply_shared_library_build_settings)
    if (ZHU_IS_TARGET_PLATFORM_OSX)
        set_target_properties(${__CurrentTargetName__} PROPERTIES
                              XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "$ENV{DEBUG_FORMAT}")
        set_target_properties(${__CurrentTargetName__} PROPERTIES
                              XCODE_ATTRIBUTE_DEPLOYMENT_POSTPROCESSING "$ENV{XCODE_DEPLOYMENT_POSTPROCESSING}")
        set_target_properties(${__CurrentTargetName__} PROPERTIES
                              XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "YES")
        set_target_properties(${__CurrentTargetName__} PROPERTIES
                              XCODE_ATTRIBUTE_GCC_INLINES_ARE_PRIVATE_EXTERN "YES")
        set_target_properties(${__CurrentTargetName__} PROPERTIES
                              XCODE_ATTRIBUTE_DYLIB_CURRENT_VERSION "1")
        set_target_properties(${__CurrentTargetName__} PROPERTIES
                              XCODE_ATTRIBUTE_DYLIB_COMPATIBILITY_VERSION "1")
    endif()
endmacro()

macro(zhu_apply_static_library_build_settings)
    if (ZHU_IS_TARGET_PLATFORM_OSX)
        set_target_properties(${__CurrentTargetName__} PROPERTIES
                              XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "YES")
        set_target_properties(${__CurrentTargetName__} PROPERTIES
                              XCODE_ATTRIBUTE_GCC_INLINES_ARE_PRIVATE_EXTERN "YES")
    endif()
endmacro()

macro(zhu_apply_link_flags)
    if(__CurrentLinkFlags__)
        get_target_property(ZHU_LINK_FLAGS ${__CurrentTargetName__} LINK_FLAGS)
        if("${ZHU_LINK_FLAGS}" STREQUAL "ZHU_LINK_FLAGS-NOTFOUND")
            set(ZHU_LINK_FLAGS "")
        endif()

        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              LINK_FLAGS "${__CurrentTargetName__} ${__CurrentLinkFlags__}")
    endif()

    if(__CurrentLinkFlagsRelease__)
        get_target_property(ZHU_LINK_FLAGS_RELEASE ${__CurrentTargetName__} LINK_FLAGS_RELEASE)
        if("${ZHU_LINK_FLAGS_RELEASE}" STREQUAL "ZHU_LINK_FLAGS_RELEASE-NOTFOUND")
            set(ZHU_LINK_FLAGS_RELEASE "")
        endif()

        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              LINK_FLAGS_RELEASE "${ZHU_LINK_FLAGS_RELEASE} ${__CurrentLinkFlagsRelease__}")
    endif()

    if(__CurrentLinkFlagsDebug__)
        get_target_property(ZHU_LINK_FLAGS_DEBUG ${__CurrentTargetName__} LINK_FLAGS_DEBUG)
        if("${ZHU_LINK_FLAGS_DEBUG}" STREQUAL "ZHU_LINK_FLAGS_DEBUG-NOTFOUND")
            set(ZHU_LINK_FLAGS_DEBUG "")
        endif()

        set_target_properties(${__CurrentTargetName__} PROPERTIES 
                              LINK_FLAGS_DEBUG "${ZHU_LINK_FLAGS_DEBUG} ${__CurrentLinkFlagsDebug__}")
    endif()
endmacro()

macro(zhu_apply_copy_resources)
    if(__CurrentResources__)
        get_target_property(_binDir ${__CurrentTargetName__} RUNTIME_OUTPUT_DIRECTORY)
        set(_filesToCopy)
        set(_dirsToCopy)

        foreach(_res ${__CurrentResources__})
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
                               COMMAND ${CMAKE_COMMAND} -E copy_files ${_filesToCopy} ${_binDir})
        endif()
        if(_dirsToCopy)
            add_custom_command(TARGET ${__CurrentTargetName__} POST_BUILD
                               COMMAND ${CMAKE_COMMAND} -E copy_directory ${_dirsToCopy} ${_binDir})
        endif()
    endif()
endmacro()

function(zhu_general_compile_options _target)
    if (MSVC)
        get_property(_disableMP TARGET ${_target} PROPERTY ZHU_DISABLE_MULTI_PROCESSOR_COMPILATION)
        if(NOT _disableMP)
            target_compile_options(${_target} PRIVATE /MP) # MultiProcessorCompilation = true
        endif()
        if(NOT CMAKE_CL_64)
            target_compile_options(${_target} PRIVATE /arch:SSE2) # EnableEnhancedInstructionSet = StreamingSIMDExtensions2
        endif()

        target_compile_options(${_target} PRIVATE /openmp-) # OpenMPSupport = false   
        target_compile_options(${_target} PRIVATE /fp:except-) # FloatingPointExceptions = false
        target_compile_options(${_target} PRIVATE /GF) # StringPooling = true
        target_compile_options(${_target} PRIVATE /Gy) # FunctionLevelLinking = true
        target_compile_options(${_target} PRIVATE /Gm-) # MinimalRebuild = false
        target_compile_options(${_target} PRIVATE /sdl) # SDLCheck = true
        target_compile_options(${_target} PRIVATE /GS) # BufferSecurityCheck = true
        

        # AdditionalOptions
        get_property(_disableRTC TARGET ${_target} PROPERTY ZHU_DISABLE_RUNTIME_CHECKS)
        if (NOT _disableRTC)
            target_compile_options(${_target} PRIVATE $<$<CONFIG:Debug>:/RTCc>)
        endif()
        get_property(_disableWPO TARGET ${_target} PROPERTY ZHU_DISABLE_WHOLE_PROGRAM_OPTIMIZATION)
        if(NOT (_disableWPO OR (DEFINED ENV{ZHU_FAST_BUILD})))
            target_compile_options(${_target} PRIVATE $<$<CONFIG:Release>:/GL>)
        endif()
        if (MSVC_VERSION GREATER_EQUAL 1915)
            target_compile_options(${_target} PRIVATE $<$<CONFIG:Debug>:/JMC>)# Just My Code Debugging = Enabled
        endif()
        target_compile_options(${_target} PRIVATE /EHsc) # Enable C++ exceptions
        target_compile_options(${_target} PRIVATE /w34370)
        target_compile_options(${_target} PRIVATE /w34701)
        target_compile_options(${_target} PRIVATE /Qfast_transcendentals)
        target_compile_options(${_target} PRIVATE /Zc:inline)
        target_compile_options(${_target} PRIVATE /Zc:throwingNew)
        target_compile_options(${_target} PRIVATE /Zc:rvalueCast)
        target_compile_options(${_target} PRIVATE /bigobj)
        target_compile_options(${_target} PRIVATE $<$<CONFIG:Debug>:/Od>) # Optimization = Disabled
        target_compile_options(${_target} PRIVATE $<$<CONFIG:Release>:/O2>) # Optimization = MaxSpeed
        target_compile_options(${_target} PRIVATE $<$<CONFIG:Debug>:/MDd>) # RuntimeLibrary = MultiThreadedDebugDLL
        target_compile_options(${_target} PRIVATE $<$<CONFIG:Release>:/MD>) # RuntimeLibrary = MultiThreadedDLL
        target_compile_options(${_target} PRIVATE $<$<CONFIG:Release>:/GT>) # EnableFiberSafeOptimizations = true
        target_compile_options(${_target} PRIVATE $<$<CONFIG:Release>:/Ot>) # FavorSizeOrSpeed = Speed
        target_compile_options(${_target} PRIVATE $<$<CONFIG:Release>:/Ob2>)  # InlineFunctionExpansion = AnySuitable
        target_compile_options(${_target} PRIVATE $<$<CONFIG:Release>:/Oi>) # IntrinsicFunctions = true
        target_compile_options(${_target} PRIVATE $<$<CONFIG:Release>:/Oy>) # OmitFramePointers = true
        target_compile_options(${_target} PRIVATE $<$<CONFIG:Release>:/Zi>) # DebugInformationFormat = ProgramDatabase
    endif()


    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_compile_options(${_target} PRIVATE -fexceptions)
        if (ZHU_IS_TARGET_PLATFORM_OSX)
        	target_compile_options(${_target} PRIVATE $<$<CONFIG:Debug>:-O0>)
            target_compile_options(${_target} PRIVATE $<$<CONFIG:Release>:-Os>)
        endif()
    endif()
endfunction()

function(zhu_general_preprocessor_definitions _target)
    if(ZHU_IS_TARGET_PLATFORM_OSX)
        target_compile_definitions(${_target} PRIVATE -DZHU_TARGET_MAC)
        target_compile_definitions(${_target} PRIVATE -D_NATIVE_WCHAR_T_DEFINED)
    endif()
    if(ZHU_IS_TARGET_PLATFORM_IOS)
        target_compile_definitions(${_target} PRIVATE -DZHU_TARGET_IOS -DZHU_TARGET_POSIX)
    endif()
    if(ZHU_IS_TARGET_PLATFORM_ANDROID)
        target_compile_definitions(${_target} PRIVATE -DZHU_TARGET_ANDROID -DZHU_TARGET_POSIX)
    endif()
    if(ZHU_IS_TARGET_PLATFORM_WINDOWS)
        target_compile_definitions(${_target} PRIVATE -DZHU_TARGET_WINDOWS)
    endif()

    if(ZHU_IS_TARGET_INTEL)
        target_compile_definitions(${_target} PRIVATE -DZHU_TARGET_INTEL)
    elseif(ZHU_IS_TARGET_ARM)
        target_compile_definitions(${_target} PRIVATE -DZHU_TARGET_ARM)
    endif()
    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_compile_definitions(${_target} PRIVATE -D__cdecl=)
    endif()

    if(MSVC)
        target_compile_definitions(${_target} PRIVATE -DWIN32)
        target_compile_definitions(${_target} PRIVATE -D_WINDOWS)
        target_compile_definitions(${_target} PRIVATE -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1)
        target_compile_definitions(${_target} PRIVATE -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT=1)
        target_compile_definitions(${_target} PRIVATE -D_ALLOW_RTCc_IN_STL)
        target_compile_definitions(${_target} PRIVATE -D_CRT_STDIO_ARBITRARY_WIDE_SPECIFIERS)
    endif()

    #Removed -D flag here because cmake treats -D as one part of preprocessor definitions somehow
    target_compile_definitions(${_target} PRIVATE $<$<CONFIG:Debug>:ZHU_CHECK_ASSERT=1>)
    target_compile_definitions(${_target} PRIVATE $<$<CONFIG:Release>:ZHU_PRODUCTION=1>)
    target_compile_definitions(${_target} PRIVATE $<$<CONFIG:Release>:_NDEBUG=1>)

    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_compile_definitions(${_target} PRIVATE $<$<CONFIG:Debug>:_DEBUG>)
        target_compile_definitions(${_target} PRIVATE $<$<CONFIG:Release>:NDEBUG>)
    endif()
endfunction()

function(zhu_general_link_options _target)
     # Make sure we append additional flags to LINK_FLAGS property
     get_target_property(ZHU_TARGET_LINK_FLAGS ${_target} LINK_FLAGS)
     if("${ZHU_TARGET_LINK_FLAGS}" STREQUAL "ZHU_TARGET_LINK_FLAGS-NOTFOUND")
         set(ZHU_TARGET_LINK_FLAGS "")
     endif()
 
     get_target_property(ZHU_TARGET_LINK_FLAGS_DEBUG ${_target} LINK_FLAGS_DEBUG)
     if("${ZHU_TARGET_LINK_FLAGS_DEBUG}" STREQUAL "ZHU_TARGET_LINK_FLAGS_DEBUG-NOTFOUND")
         set(ZHU_TARGET_LINK_FLAGS_DEBUG "")
     endif()
     get_target_property(ZHU_TARGET_LINK_FLAGS_RELEASE ${_target} LINK_FLAGS_RELEASE)
     if("${ZHU_TARGET_LINK_FLAGS_RELEASE}" STREQUAL "ZHU_TARGET_LINK_FLAGS_RELEASE-NOTFOUND")
         set(ZHU_TARGET_LINK_FLAGS_RELEASE "")
     endif()
 
     if(MSVC)
         list(APPEND ZHU_TARGET_LINK_FLAGS "/CLRIMAGETYPE:IJW") # CLRImageType = ForceIJWImage
         list(APPEND ZHU_TARGET_LINK_FLAGS "/CLRTHREADATTRIBUTE:STA") # CLRThreadAttribute = STAThreadingAttribute
         list(APPEND ZHU_TARGET_LINK_FLAGS "/OPT:REF") # OptimizeReferences = true
         list(APPEND ZHU_TARGET_LINK_FLAGS "/CLRUNMANAGEDCODECHECK") # CLRUnmanagedCodeCheck = true
         list(APPEND ZHU_TARGET_LINK_FLAGS "/INCREMENTAL:NO")
         # We use declspec(dllexport) to export functions and data from dlls on Win and UWP platforms. 
         # We do not use declspec(dllimport) to import functions but we do use it for data. 
         list(APPEND ZHU_TARGET_LINK_FLAGS "/IGNORE:4049,4217")

         set(ZHU_TARGET_LINK_FLAGS_RELEASE "${ZHU_TARGET_LINK_FLAGS_RELEASE} /NXCOMPAT") # DataExecutionPrevention=true
         set(ZHU_TARGET_LINK_FLAGS_RELEASE "${ZHU_TARGET_LINK_FLAGS_RELEASE} /OPT:ICF") # EnableCOMDATFolding = true
         if(NOT (DEFINED ENV{ZHU_FAST_BUILD}))
             set(ZHU_TARGET_LINK_FLAGS_RELEASE "${ZHU_TARGET_LINK_FLAGS_RELEASE} /LTCG")
         endif()
     endif()
 
     set_target_properties(${_target} PROPERTIES LINK_FLAGS "${ZHU_TARGET_LINK_FLAGS}")
     set_target_properties(${_target} PROPERTIES LINK_FLAGS_DEBUG "${ZHU_TARGET_LINK_FLAGS_DEBUG}")
     set_target_properties(${_target} PROPERTIES LINK_FLAGS_RELEASE "${ZHU_TARGET_LINK_FLAGS_RELEASE}")
endfunction()

function(zhu_general_disable_specific_warnings _target)
     if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_compile_options(${_target} PRIVATE -Wno-backslash-newline-escape
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
endfunction()

function(zhu_target_treat_warnings_as_errors _target)
    if(MSVC)
        target_compile_options(${_target} PRIVATE /WX)
        #TreatSpecificWarningsAsErrors = 4265
        if(NOT "${_target}" STREQUAL "boost")
            #target_compile_options(${_target} PRIVATE /we4265)
        endif()
    endif()
    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_compile_options(${_target} PRIVATE -Werror)
        # CMake hard coded "-Wmost" for Xcode which will generate more warnings than main stream build.
        # So, we disable "-Wmost" for the moment. Long term solution is to fix the code so that no warning generated.
        target_compile_options(${_target} PRIVATE -Wno-most)
    endif()
endfunction()



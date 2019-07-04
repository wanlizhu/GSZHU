add_library(ZHUENGINE)
add_library(ZHU::Engine ALIAS ZHUENGINE)

# This file is quite big, so for readability we include instead...
include(CMakeSources.cmake)

target_compile_features(ZHUENGINE PUBLIC cxx_std_17)
if(BUILD_SHARED_LIBS)
	target_compile_definitions(ZHUENGINE PRIVATE ZHU_SHARED_EXPORTS)
else()
	target_compile_definitions(ZHUENGINE PUBLIC ZHU_STATIC)
endif()

# Compile definitions
target_compile_definitions(ZHUENGINE
        PRIVATE
        $<$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>:ZHU_BUILD_32BIT>
		$<$<EQUAL:8,${CMAKE_SIZEOF_VOID_P}>:ZHU_BUILD_64BIT>
        $<$<PLATFORM_ID:Windows>:ZHU_BUILD_WINDOWS WIN32_LEAN_AND_MEAN NOMINMAX>
        $<$<PLATFORM_ID:Linux>:ZHU_BUILD_LINUX>
        $<$<PLATFORM_ID:Darwin>:ZHU_BUILD_MACOS>
        $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS>
)

# Compile options
target_compile_options(ZHUENGINE
		PUBLIC
		$<$<CXX_COMPILER_ID:MSVC>:/wd4251 /wd4592>
		PRIVATE
		$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-msse4.1>
		$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>>:/arch:SSE2>
)

# Include directories
target_include_directories(ZHUENGINE
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
)

# Link libraries
target_link_libraries(ZHUENGINE
		PUBLIC
        Eigen
		$<$<PLATFORM_ID:Windows>:ws2_32>
		$<$<PLATFORM_ID:Windows>:dbghelp>
		PRIVATE
        Assimp
        SDL2
        JSON
        $<$<BOOL:${ENABLE_VULKAN}>:Vulkan::Vulkan>
)

# Target properties
set_target_properties(ZHUENGINE PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
		FOLDER "ZHUENGINE"
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

# Copy binary files to output directory
if (ZHU_SDK_DLLS)
        add_custom_command(TARGET ZHUENGINE 
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ZHU_SDK_DLLS} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>"
        )
endif()

# Install headers
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
		FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hpp"
)
# Install binary files
install(TARGETS ZHUENGINE
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
                
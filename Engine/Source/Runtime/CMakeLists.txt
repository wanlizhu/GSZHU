add_library(ZHUENGINE)
add_library(ZHU::Engine ALIAS ZHUENGINE)
include(CMakeSources.cmake)

target_compile_features(ZHUENGINE PUBLIC cxx_std_17)
if(BUILD_SHARED_LIBS)
	target_compile_definitions(ZHUENGINE PRIVATE ZHU_SHARED_EXPORTS)
else()
	target_compile_definitions(ZHUENGINE PUBLIC ZHU_STATIC)
endif()

# Compile definitions
target_compile_definitions(ZHUENGINE
    PUBLIC 
        $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS>
    PRIVATE
        $<$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>:ZHU_BUILD_32BIT>
		$<$<EQUAL:8,${CMAKE_SIZEOF_VOID_P}>:ZHU_BUILD_64BIT>
        $<$<PLATFORM_ID:Windows>:ZHU_BUILD_WINDOWS WIN32_LEAN_AND_MEAN NOMINMAX>
        $<$<PLATFORM_ID:Linux>:ZHU_BUILD_LINUX>
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
		"${CMAKE_CURRENT_SOURCE_DIR}/Public"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

# Link libraries
find_package(Vulkan)
target_link_libraries(ZHUENGINE
	PUBLIC
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
	FOLDER "ZHUENGINE"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set(DependentDlls
    "${ZHU_ENGINE_DIR}/Libraries/Assimp/bin/x64/assimp-vc140-mt.dll"
    "${ZHU_ENGINE_DIR}/Libraries/SDL2/bin/x64/SDL2.dll"
)
add_custom_command(TARGET ZHUENGINE 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DependentDlls} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>"
)


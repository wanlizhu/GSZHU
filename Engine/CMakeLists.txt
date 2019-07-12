add_library(Engine)
file(GLOB_RECURSE SOURCE_LIST "*.cpp" "*.c")
file(GLOB_RECURSE HEADER_LIST "*.h" "*.hpp")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_LIST})

target_sources(Engine PRIVATE ${HEADER_LIST})
target_sources(Engine PRIVATE ${SOURCE_LIST})

include(PrecompiledHeader)
add_precompiled_header(Engine engine_pch.h SOURCE_CXX engine_pch.cpp)




target_compile_features(Engine PUBLIC cxx_std_17)
if(BUILD_SHARED_LIBS)
	target_compile_definitions(Engine PRIVATE ZHU_SHARED_EXPORTS)
else()
	target_compile_definitions(Engine PUBLIC ZHU_STATIC)
endif()

# Compile definitions
target_compile_definitions(Engine
    PUBLIC 
        $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS>
    PRIVATE
        $<$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>:ZHU_BUILD_32BIT>
		$<$<EQUAL:8,${CMAKE_SIZEOF_VOID_P}>:ZHU_BUILD_64BIT>
        $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN NOMINMAX>
        $<$<PLATFORM_ID:Linux>:ZHU_BUILD_LINUX>
)

# Compile options
target_compile_options(Engine
	PUBLIC
		$<$<CXX_COMPILER_ID:MSVC>:/wd4251 /wd4592>
	PRIVATE
		$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-msse4.1>
		$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>>:/arch:SSE2>
)

# Include directories
target_include_directories(Engine
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Link libraries
find_package(Vulkan)
target_link_libraries(Engine
    PUBLIC
        Eigen
	PRIVATE
        Assimp
        SDL2
        JSON
        TinyXML2
        $<$<BOOL:${ENABLE_VULKAN}>:Vulkan::Vulkan>
)

# Target properties
set_target_properties(Engine PROPERTIES
	FOLDER "ENGINE"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Post-Build event
file(GLOB_RECURSE ZHU_RT_REQUIRED_DLLS "../External/Library/*.dll")
add_custom_command(TARGET Engine
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ZHU_RT_REQUIRED_DLLS} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>"
)

install(TARGETS Engine
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES "${HEADER_LIST}"
    DESTINATION "include"
)


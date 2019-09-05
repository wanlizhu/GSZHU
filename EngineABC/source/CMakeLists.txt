cmake_minimum_required(VERSION 2.8.11)

set(TargetName EngineABC)
project(${TargetName})

# Variables
set(AdditionalLibraryList
    $<$<BOOL:${USE_VULKAN}>:Vulkan::Vulkan>)

# The version number.
set(${TargetName}_VERSION_MAJOR 1)
set(${TargetName}_VERSION_MINOR 0)

# Configure a header file to pass some of the CMake settings
# to the source code
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/EngineConfig.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/EngineConfig.h")

# Since the configured file will be written into the binary tree,
# we must include that directory
set(AdditionalIncludeList
    "${CMAKE_CURRENT_SOURCE_DIR}/core/public"
    "${CMAKE_CURRENT_SOURCE_DIR}/geometry/public"
    "${CMAKE_CURRENT_SOURCE_DIR}/raytracing/public"
    "${CMAKE_CURRENT_SOURCE_DIR}/render/public")

add_library(${TargetName})
include(FindSourceFiles.cmake)
target_sources(${TargetName} PUBLIC  ${PublicSourceFileList} 
                             PRIVATE ${PrivateSourceFileList})
target_include_directories(${TargetName} PRIVATE ${AdditionalIncludeList} "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(${TargetName} PRIVATE ${AdditionalLibraryList})
set_target_properties(${TargetName} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
target_compile_definitions(${TargetName} 
    PRIVATE
    $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN 
                             NOMINMAX>
    $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS 
                              _CRT_SECURE_NO_WARNINGS 
                              _WINSOCK_DEPRECATED_NO_WARNINGS>)
target_compile_options(${TargetName} 
    PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:/wd4251 /wd4592>
	PRIVATE
	$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>>:/arch:SSE2>)

# Copy files after build
add_custom_command(TARGET ${TargetName}  
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    "${CMAKE_CURRENT_SOURCE_DIR}/../data" 
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>")

# Install
install(TARGETS ${TargetName} 
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(DIRECTORY ${AdditionalIncludeList} DESTINATION include)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/EngineConfig.h" DESTINATION include)


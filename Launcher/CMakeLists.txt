cmake_minimum_required(VERSION 2.8.11)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON) # To allow making project folders in IDEs
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(LauncherTarget Launcher)
project(${LauncherTarget})

# The version number.
set(${LauncherTarget}_VERSION_MAJOR 1)
set(${LauncherTarget}_VERSION_MINOR 0)

add_library(${LauncherTarget})
include(AddSourceFiles.cmake) # set SourceFiles
target_sources(${LauncherTarget} PRIVATE ${SourceFiles})
target_include_directories(${LauncherTarget} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source")
target_link_libraries(${LauncherTarget} PRIVATE ${EngineABCTarget})
set_target_properties(${LauncherTarget} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
target_compile_definitions(${LauncherTarget} 
    PRIVATE
    $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN 
                             NOMINMAX>
    $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS 
                              _CRT_SECURE_NO_WARNINGS 
                              _WINSOCK_DEPRECATED_NO_WARNINGS>)
target_compile_options(${LauncherTarget} 
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/wd4251 /wd4592>
	$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>>:/arch:SSE2>)
#target_link_options(...)

# Copy files after build
add_custom_command(TARGET ${LauncherTarget}  
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    "${CMAKE_CURRENT_SOURCE_DIR}/data" 
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>")

# Install
install(TARGETS ${LauncherTarget} 
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

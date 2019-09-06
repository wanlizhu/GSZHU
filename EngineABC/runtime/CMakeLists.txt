cmake_minimum_required(VERSION 2.8.11)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON) # To allow making project folders in IDEs
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

option(BUILD_SHARED_LIBS "Build as a dynamically linked library" ON)
option(USE_MY_BOOST      "Use a customized minimum-sized Boost library" ON)
option(USE_DX12          "Use DX12 as the backend rendering API" ON)
option(USE_VULKAN        "Use Vulkan as the backend rendering API" OFF)

set(EngineABCTarget EngineABC)
project(${EngineABCTarget})

# The version number.
set(${EngineABCTarget}_VERSION_MAJOR 1)
set(${EngineABCTarget}_VERSION_MINOR 0)

add_library(${EngineABCTarget})
include(AddSourceFiles.cmake) # set SourceFiles
target_sources(${EngineABCTarget} PRIVATE ${SourceFiles})
target_include_directories(${EngineABCTarget} PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include" 
                                              PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source")
target_link_libraries(${EngineABCTarget} PRIVATE 
    External::EIGEN
    External::GLFW
    External::GLOG
    External::STB
    $<$<BOOL:${USE_VULKAN}>:Vulkan::Vulkan>)
set_target_properties(${EngineABCTarget} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
target_compile_definitions(${EngineABCTarget} 
    PUBLIC
    UNICODE _UNICODE
    PRIVATE
    $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN 
                             NOMINMAX>
    $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS 
                              _CRT_SECURE_NO_WARNINGS 
                              _WINSOCK_DEPRECATED_NO_WARNINGS>)
target_compile_options(${EngineABCTarget} 
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/wd4251 /wd4592>
	$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>>:/arch:SSE2>)
#target_link_options(...)

# Copy files after build
add_custom_command(TARGET ${EngineABCTarget}  
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    "${CMAKE_CURRENT_SOURCE_DIR}/../data" 
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>")

# Install
install(TARGETS ${EngineABCTarget} 
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include" DESTINATION include)



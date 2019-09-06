cmake_minimum_required(VERSION 2.8.11)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON) # To allow making project folders in IDEs
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(RHITarget RHI)
project(${RHITarget})

# The version number.
set(${RHITarget}_VERSION_MAJOR 1)
set(${RHITarget}_VERSION_MINOR 0)

add_library(${RHITarget})
include(AddSourceFiles.cmake) # set SourceFiles
target_sources(${RHITarget} PRIVATE ${SourceFiles})
target_include_directories(${RHITarget} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source" 
                                        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source")
target_link_libraries(${RHITarget} PRIVATE 
    External::EIGEN
    External::GLOG
    External::STB)
set_target_properties(${RHITarget} PROPERTIES
    FOLDER "Plugins"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
target_compile_definitions(${RHITarget} 
    PRIVATE
    UNICODE _UNICODE
    $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN 
                             NOMINMAX>
    $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS 
                              _CRT_SECURE_NO_WARNINGS 
                              _WINSOCK_DEPRECATED_NO_WARNINGS>)
target_compile_options(${RHITarget} 
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/wd4251 /wd4592>
	$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>>:/arch:SSE2>)
#target_link_options(...)

# Install
install(TARGETS ${RHITarget} 
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include" DESTINATION include)



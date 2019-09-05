cmake_minimum_required(VERSION 2.8.11)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

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

add_subdirectory(source)
add_subdirectory(external)
add_subdirectory(plugins)
add_subdirectory(test)
cmake_minimum_required(VERSION 2.8.11)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON) # To allow making project folders in IDEs
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(LauncherTargetName Launcher)
set(CurrentTarget ${LauncherTargetName})
project(${CurrentTarget})

# Variables
set(AdditionalLibraryList ${EngineTargetName})

# Include list
set(AdditionalIncludeList "${CMAKE_CURRENT_SOURCE_DIR}/external")

add_executable(${CurrentTarget})
target_sources(${CurrentTarget} PRIVATE "source/main.cpp")
target_include_directories(${CurrentTarget} PRIVATE ${AdditionalIncludeList})
target_link_libraries(${CurrentTarget} PRIVATE ${AdditionalLibraryList})
set_target_properties(${CurrentTarget} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

# Copy files after build
add_custom_command(TARGET ${CurrentTarget}  
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    "${CMAKE_CURRENT_SOURCE_DIR}/data" 
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>")

# Install
install(TARGETS ${CurrentTarget} 
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")


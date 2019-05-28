cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# version information
set(GSZHU_MAJOR_VERSION 1)
set(GSZHU_MINOR_VERSION 2)
set(GSZHU_PATCH_VERSION 3)
set(GSZHU_SOVERSION 100)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")


# define project 
project(GSZHU)
set(GSZHU_VERSION ${GSZHU_MAJOR_VERSION}.${GSZHU_MINOR_VERSION}.${GSZHU_PATCH_VERSION})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


# customized variables
set(GSZHU_ENGINE_DEFINES "")
set(GSZHU_ENGINE_LINK_LIBS "")
set(GSZHU_ENGINE_INCLUDES "source" "thirdparty/include")
set(GSZHU_WARNINGS_TO_DISABLE "")
option(GSZHU_BUILD_STATIC "Build static library" ON)
option(GSZHU_DISABLE_WARNINGS "Turn off various warnings." ON)
option(GSZHU_BUILD_EDITOR "Build editor application." ON)
option(GSZHU_BUILD_TEST "Build unit test" ON)


# find dependent libraries
## find Vulkan
find_package(Vulkan)


# scan GSZHU source files
file(GLOB_RECURSE GSZHU_ENGINE_SRCS source/*.cpp)
file(GLOB_RECURSE GSZHU_ENGINE_HDRS source/*.h)
file(GLOB_RECURSE GSZHU_SHADERS_GLSL resources/shaders/*.glsl)
file(GLOB_RECURSE GSZHU_SHADERS_HLSL resources/shaders/*.hlsl)
file(GLOB_RECURSE GSZHU_ENGINE_INTERFACES include/GSZHU/*.h)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/GSZHU PREFIX "Interface Files" FILES ${GSZHU_ENGINE_INTERFACES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX "Source Files" FILES ${GSZHU_ENGINE_SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX "Header Files" FILES ${GSZHU_ENGINE_HDRS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/glsl PREFIX "Shader Files/GLSL" FILES ${GSZHU_SHADERS_GLSL})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/hlsl PREFIX "Shader Files/HLSL" FILES ${GSZHU_SHADERS_HLSL})
set(GSZHU_RESOURCE_FILES "")
if (WIN32 AND MSVC) 
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-DNOMINMAX)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DGSZHU_OS_WINDOWS)
    list(APPEND GSZHU_RESOURCE_FILES "resources/Win32/GSZHU.rc" 
                                     "resources/Win32/app_icon.ico" 
                                     "resources/Win32/resource.h")
    source_group("Resources" FILES ${GSZHU_RESOURCE_FILES})

    if (GSZHU_DISABLE_WARNINGS)
        foreach(warning ${GSZHU_WARNINGS_TO_DISABLE})
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd${warning}")
        endforeach()
    endif()
endif()


# define GSZHU targets
set(GSZHU_ENGINE "GSZHU")
set(GSZHU_ENGINE_BUILD_FILES ${GSZHU_ENGINE_SRCS} 
                             ${GSZHU_ENGINE_HDRS}
                             ${GSZHU_ENGINE_INTERFACES}
                             ${GSZHU_SHADERS_GLSL}
                             ${GSZHU_SHADERS_HLSL}
                             ${GSZHU_RESOURCE_FILES})
if(GSZHU_BUILD_STATIC)
    add_library(${GSZHU_ENGINE} STATIC ${GSZHU_ENGINE_BUILD_FILES})  
else()
    add_library(${GSZHU_ENGINE} SHARED ${GSZHU_ENGINE_BUILD_FILES})
    list(APPEND GSZHU_ENGINE_DEFINES "-DGSZHU_EXPORT_SHARED")
endif()

target_compile_definitions(${GSZHU_ENGINE} PRIVATE ${GSZHU_ENGINE_DEFINES})
target_include_directories(${GSZHU_ENGINE} PRIVATE ${GSZHU_ENGINE_INCLUDES} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_directories(${GSZHU_ENGINE} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lib")
target_link_libraries(${GSZHU_ENGINE} PRIVATE ${GSZHU_ENGINE_LINK_LIBS})


# copy .dll to bin
file(GLOB_RECURSE DLL_FILES "thirdparty/bin/*.dll")
add_custom_command(TARGET ${GSZHU_ENGINE} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy "${DLL_FILES}" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>"
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


# install target GSZHU
install(TARGETS ${GSZHU_ENGINE} DESTINATION "GSZHU/bin")
install(FILES ${DLL_FILES} DESTINATION "GSZHU/bin")
install(FILES ${GSZHU_ENGINE_INTERFACES} DESTINATION "GSZHU/include")


if (GSZHU_BUILD_EDITOR)
    add_subdirectory(editor)
endif()

if (GSZHU_BUILD_TEST)
    add_subdirectory(test)
endif()
 
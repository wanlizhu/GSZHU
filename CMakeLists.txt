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
set(GSZHU_ENGINE_INCLUDES "source/GSZHU" "thirdparty/include")
set(GSZHU_WARNINGS_TO_DISABLE 4244 4251 4267 4275)
option(GSZHU_BUILD_SHARED "Build static library" ON)
option(GSZHU_DISABLE_WARNINGS "Turn off various warnings." ON)
option(GSZHU_ENABLE_SIMD_VECTOR "Build SIMD vector" ON)
option(GSZHU_BUILD_SAMPLE "Build sample application." ON)
option(GSZHU_BUILD_TEST "Build unit test" ON)


# find dependent libraries
## find Vulkan
find_package(Vulkan REQUIRED)
set(SDL2_BUILDING_LIBRARY ON)


# scan GSZHU source files
file(GLOB_RECURSE GSZHU_ENGINE_SRCS source/*.cpp)
file(GLOB_RECURSE GSZHU_ENGINE_HDRS source/*.h)
file(GLOB_RECURSE GSZHU_ENGINE_INTERFACES include/GSZHU/*.h)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/GSZHU PREFIX "Interface Files" FILES ${GSZHU_ENGINE_INTERFACES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source/GSZHU PREFIX "Source Files/GSZHU" FILES ${GSZHU_ENGINE_SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source/GSZHU PREFIX "Header Files/GSZHU" FILES ${GSZHU_ENGINE_HDRS})


# define GSZHU targets
set(GSZHU_ENGINE "GSZHU")
set(GSZHU_ENGINE_BUILD_FILES ${GSZHU_ENGINE_SRCS} ${GSZHU_ENGINE_HDRS} ${GSZHU_ENGINE_INTERFACES})
if(GSZHU_BUILD_SHARED)
    add_library(${GSZHU_ENGINE} SHARED ${GSZHU_ENGINE_BUILD_FILES})
    list(APPEND GSZHU_ENGINE_DEFINES "-DGSZHU_EXPORT_SHARED")
else()
    add_library(${GSZHU_ENGINE} STATIC ${GSZHU_ENGINE_BUILD_FILES})
endif()

target_compile_definitions(${GSZHU_ENGINE} PRIVATE ${GSZHU_ENGINE_DEFINES})
target_include_directories(${GSZHU_ENGINE} PRIVATE ${GSZHU_ENGINE_INCLUDES} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_directories(${GSZHU_ENGINE} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lib")
target_link_libraries(${GSZHU_ENGINE} PRIVATE ${GSZHU_ENGINE_LINK_LIBS})

if (GSZHU_ENABLE_SIMD_VECTOR)
    add_definitions(-DGSZHU_ENABLE_SIMD_VECTOR)
endif()

if (WIN32 AND MSVC) 
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-DNOMINMAX)
    add_definitions(-DGSZHU_OS_WINDOWS)
    
    if (GSZHU_DISABLE_WARNINGS)
        foreach(warning ${GSZHU_WARNINGS_TO_DISABLE})
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd${warning}")
        endforeach()
    endif()
endif()


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


if (GSZHU_BUILD_SAMPLE)
    # add samples
    add_subdirectory(sample)
endif()

if (GSZHU_BUILD_TEST)
    # add tests
    add_subdirectory(test)
endif()
 
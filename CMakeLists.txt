cmake_minimum_required(VERSION 3.1)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(KingsLynn)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${${CMAKE_CURRENT_SOURCE_DIR}/cmake}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(KL_GEN_TEST ON)

# scan source files
file(GLOB_RECURSE ENGINE_HDRS source/engine/*.h)
file(GLOB_RECURSE ENGINE_SRCS source/engine/*.cpp)
file(GLOB_RECURSE EDITOR_HDRS source/editor/*.h)
file(GLOB_RECURSE EDITOR_SRCS source/editor/*.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source/engine PREFIX "Header Files/engine" FILES ${ENGINE_HDRS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source/engine PREFIX "Source Files/engine" FILES ${ENGINE_SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source/editor PREFIX "Header Files/editor" FILES ${EDITOR_HDRS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source/editor PREFIX "Source Files/editor" FILES ${EDITOR_SRCS})

if (WIN32)

endif()

# engine target definition
add_subdirectory(thirdparty/glad)
add_library(engine STATIC ${ENGINE_HDRS} ${ENGINE_SRCS})
target_compile_definitions(engine PRIVATE "ARCH=x64" 
                                  PUBLIC "SDL_MAIN_HANDLED")
target_include_directories(engine PRIVATE "source/engine" "thirdparty"
                                  PUBLIC "thirdparty/SDL2")
target_link_directories(engine PUBLIC "libs")
target_link_libraries(engine PUBLIC "SDL2" glad)

# copy .dll to bin
file(GLOB_RECURSE DLL_FILES libs/*.dll)
add_custom_command(TARGET engine POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy "${DLL_FILES}" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>"
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# editor target definition
add_executable(editor ${EDITOR_HDRS} ${EDITOR_SRCS})
target_include_directories(editor PRIVATE source/engine)
target_link_libraries(editor PRIVATE engine)

# engine-test target definition
if (KL_GEN_TEST)
    add_subdirectory(thirdparty/gtest)
    file(GLOB_RECURSE TEST_SRCS tests/src/*.cpp tests/src/*.h)
    add_executable(engine-test ${TEST_SRCS})
    target_include_directories(engine-test PRIVATE tests tests/src source/engine)
    target_link_libraries(engine-test PRIVATE gtest engine)
endif()

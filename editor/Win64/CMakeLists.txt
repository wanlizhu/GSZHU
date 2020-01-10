add_executable(Win64Editor "${RSD}/dummy.cpp")
SetOutputDirs(Win64Editor)

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE _sourceFiles_ "${CSD}/*.cpp" "${CSD}/*.c")
file(GLOB_RECURSE _headerFiles_ "${CSD}/*.h" "${CSD}/*.hpp")
source_group(TREE "${CSD}"  PREFIX "Source Files" FILES ${_sourceFiles_})
source_group(TREE "${CSD}"  PREFIX "Header Files" FILES ${_headerFiles_})
target_sources(Win64Editor PRIVATE ${_sourceFiles_} ${_headerFiles_})

target_include_directories(Win64Editor PRIVATE "${CSD}")
target_link_libraries(Win64Editor PRIVATE Wanlix)
target_compile_definitions(Win64Editor PRIVATE "_UNICODE" "UNICODE")

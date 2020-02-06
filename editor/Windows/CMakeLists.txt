add_executable(WindowsEditor "${RSD}/dummy.cpp")
SetOutputDirs(WindowsEditor)

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE _sourceFiles_ "${CSD}/*.cpp" "${CSD}/*.c")
file(GLOB_RECURSE _headerFiles_ "${CSD}/*.h" "${CSD}/*.hpp")
source_group(TREE "${CSD}"  PREFIX "Source Files" FILES ${_sourceFiles_})
source_group(TREE "${CSD}"  PREFIX "Header Files" FILES ${_headerFiles_})
target_sources(WindowsEditor PRIVATE ${_sourceFiles_} ${_headerFiles_})

target_include_directories(WindowsEditor PRIVATE "${CSD}")
target_link_libraries(WindowsEditor PRIVATE Wanli)
target_compile_definitions(WindowsEditor PRIVATE "_UNICODE" "UNICODE")

add_executable(Editor "${RSD}/dummy.cpp")

SetTargetOutputDirectories(Editor)
GetCurrentDirectorySourceFiles(_interfaces_ _headers_ _sources_)
target_sources(Editor PRIVATE ${_headers_} ${_sources_})

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(Editor PRIVATE "${CSD}/include")
target_include_directories(Editor PRIVATE "${CSD}/source")
target_include_directories(Editor PRIVATE "${CSD}")
target_link_libraries(Editor PRIVATE Wanlix)
target_link_libraries(Editor PRIVATE Core3D) # for debug only
target_compile_definitions(Editor PRIVATE "_UNICODE" "UNICODE")

get_target_property(_outdir_ Editor RUNTIME_OUTPUT_DIRECTORY)
message(STATUS "Target: Editor -> ${_outdir_}" )
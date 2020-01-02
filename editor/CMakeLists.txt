add_executable(WanlixEditor "${RSD}/dummy.cpp")

SetTargetOutputDirectories(WanlixEditor)
GetCurrentDirectorySourceFiles(_interfaces_ _headers_ _sources_)
target_sources(WanlixEditor PRIVATE ${_headers_} ${_sources_})

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(WanlixEditor PRIVATE "${CSD}/include")
target_include_directories(WanlixEditor PRIVATE "${CSD}/source")
target_include_directories(WanlixEditor PRIVATE "${CSD}")
target_link_libraries(WanlixEditor PRIVATE Wanlix)
target_compile_definitions(WanlixEditor PRIVATE "_UNICODE" "UNICODE")

get_target_property(_outdir_ WanlixEditor RUNTIME_OUTPUT_DIRECTORY)
message(STATUS "Target: WanlixEditor -> ${_outdir_}" )
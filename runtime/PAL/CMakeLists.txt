add_library(PAL STATIC "${RSD}/dummy.cpp")

SetTargetOutputDirectories(PAL)
GetCurrentDirectorySourceFiles(_interfaces_ _headers_ _sources_)
target_sources(PAL PRIVATE ${_interfaces_} ${_headers_} ${_sources_})

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(PAL PUBLIC "${CSD}/include")
target_include_directories(PAL PRIVATE "${RSD}/include")
target_include_directories(PAL PRIVATE "${CSD}/source")
target_compile_definitions(PAL PRIVATE "_UNICODE" "UNICODE")

get_target_property(_outdir_ PAL LIBRARY_OUTPUT_DIRECTORY)
message(STATUS "Target: PAL -> ${_outdir_}" )
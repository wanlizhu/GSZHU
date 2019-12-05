add_library(GAL STATIC "${RSD}/dummy.cpp")

SetTargetOutputDirectories(GAL)
GetCurrentDirectorySourceFiles(_interfaces_ _headers_ _sources_)
target_sources(GAL PRIVATE ${_interfaces_} ${_headers_} ${_sources_})

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(GAL PUBLIC "${CSD}/include" PRIVATE "${CSD}/source")
target_compile_definitions(GAL PRIVATE "_UNICODE" "UNICODE")

get_target_property(_outdir_ GAL LIBRARY_OUTPUT_DIRECTORY)
message(STATUS "Target: GAL -> ${_outdir_}" )
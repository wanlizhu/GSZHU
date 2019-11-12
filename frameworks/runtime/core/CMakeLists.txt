add_subdirectory(modules/dm)
add_subdirectory(modules/ga)
add_subdirectory(modules/gs)

add_library(WanlixCore STATIC "${RSD}/dummy.cpp")

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE _headers_    "${CSD}/include/*.h" "${CSD}/include/*.hpp")
file(GLOB_RECURSE _sources_    "${CSD}/source/*.c"  "${CSD}/source/*.cpp")
source_group(TREE "${CSD}/include"  PREFIX "Header Files" FILES ${_headers_})
source_group(TREE "${CSD}/source"   PREFIX "Source Files" FILES ${_sources_})
target_sources(WanlixCore PRIVATE ${_headers_} ${_sources_})

target_include_directories(WanlixCore PUBLIC "${CSD}/include" PRIVATE "${CSD}/source")
target_link_libraries(WanlixCore PRIVATE DM)
target_link_libraries(WanlixCore PRIVATE GA)
target_link_libraries(WanlixCore PRIVATE GS)

get_target_property(_outdir_ WanlixCore LIBRARY_OUTPUT_DIRECTORY)
message(STATUS "Target: WanlixCore -> ${_outdir_}" )
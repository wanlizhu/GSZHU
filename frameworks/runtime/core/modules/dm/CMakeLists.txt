add_library(WanlixCoreDM STATIC "${RSD}/dummy.cpp")

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE _headers_    "${CSD}/include/*.h" "${CSD}/include/*.hpp")
file(GLOB_RECURSE _sources_    "${CSD}/source/*.c"  "${CSD}/source/*.cpp")
source_group(TREE "${CSD}/include"  PREFIX "Header Files" FILES ${_headers_})
source_group(TREE "${CSD}/source"   PREFIX "Source Files" FILES ${_sources_})
target_sources(WanlixCoreDM PRIVATE ${_headers_} ${_sources_})

target_include_directories(WanlixCoreDM PUBLIC "${CSD}/include" PRIVATE "${CSD}/source")
target_link_libraries(WanlixCoreDM PRIVATE CONAN_PKG::jsonformoderncpp)
target_link_libraries(WanlixCoreDM PRIVATE CONAN_PKG::sqlite3)
target_link_libraries(WanlixCoreDM PRIVATE CONAN_PKG::stb)

get_target_property(_outdir_ WanlixCoreDM LIBRARY_OUTPUT_DIRECTORY)
message(STATUS "Target: WanlixCoreDM -> ${_outdir_}" )
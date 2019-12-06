add_library(DMS STATIC "${RSD}/dummy.cpp")

SetTargetOutputDirectories(DMS)
GetCurrentDirectorySourceFiles(_interfaces_ _headers_ _sources_)
target_sources(DMS PRIVATE ${_interfaces_} ${_headers_} ${_sources_})

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(DMS PUBLIC "${CSD}/include")
target_include_directories(DMS PRIVATE "${CSD}/source")
target_compile_definitions(DMS PRIVATE "_UNICODE" "UNICODE")
if (NOT WANLIX_CODE_REVIEW_MODE)
    target_link_libraries(DMS PRIVATE CONAN_PKG::Assimp)
    target_link_libraries(DMS PRIVATE CONAN_PKG::Eigen3)
endif()
target_link_libraries(DMS PRIVATE RHI)
target_link_libraries(DMS PRIVATE PAL)

get_target_property(_outdir_ DMS LIBRARY_OUTPUT_DIRECTORY)
message(STATUS "Target: DMS -> ${_outdir_}" )
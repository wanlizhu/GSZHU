add_library(SIM STATIC "${RSD}/dummy.cpp")

SetTargetOutputDirectories(SIM)
GetCurrentDirectorySourceFiles(_interfaces_ _headers_ _sources_)
target_sources(SIM PRIVATE ${_interfaces_} ${_headers_} ${_sources_})

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(SIM PUBLIC "${CSD}/include")
target_include_directories(SIM PRIVATE "${CSD}/source")
target_compile_definitions(SIM PRIVATE "_UNICODE" "UNICODE")
if (NOT WANLIX_CODE_REVIEW_MODE)
    target_link_libraries(SIM PRIVATE CONAN_PKG::Assimp)
    target_link_libraries(SIM PRIVATE CONAN_PKG::Eigen3)
endif()
target_link_libraries(SIM PRIVATE RHI)
target_link_libraries(SIM PRIVATE PAL)

get_target_property(_outdir_ SIM LIBRARY_OUTPUT_DIRECTORY)
message(STATUS "Target: SIM -> ${_outdir_}" )
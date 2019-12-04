add_library(Core3D STATIC "${RSD}/dummy.cpp")

SetTargetOutputDirectories(Core3D)
GetCurrentDirectorySourceFiles(_interfaces_ _headers_ _sources_)
target_sources(Core3D PRIVATE ${_interfaces_} ${_headers_} ${_sources_})

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(Core3D PUBLIC "${CSD}/include" PRIVATE "${CSD}/source")
target_compile_definitions(Core3D PRIVATE "_UNICODE" "UNICODE")
if (NOT WANLIX_CODE_REVIEW_MODE)
    target_link_libraries(Core3D PRIVATE CONAN_PKG::Assimp)
    target_link_libraries(Core3D PRIVATE CONAN_PKG::Eigen3)
endif()
target_link_libraries(Core3D PRIVATE RHI)
target_link_libraries(Core3D PRIVATE PAL)

get_target_property(_outdir_ Core3D LIBRARY_OUTPUT_DIRECTORY)
message(STATUS "Target: Core3D -> ${_outdir_}" )
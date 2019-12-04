add_library(RHI STATIC "${RSD}/dummy.cpp")

SetTargetOutputDirectories(RHI)
GetCurrentDirectorySourceFiles(_interfaces_ _headers_ _sources_)
target_sources(RHI PRIVATE ${_interfaces_} ${_headers_} ${_sources_})

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(RHI PUBLIC "${CSD}/include" PRIVATE "${CSD}/source")
target_compile_definitions(RHI PRIVATE "_UNICODE" "UNICODE")
target_compile_definitions(RHI PRIVATE $<$<BOOL:WANLIX_ENABLE_D3D12>:"RHI_ENABLE_D3D12">)
target_compile_definitions(RHI PRIVATE $<$<BOOL:WANLIX_ENABLE_VULKAN>:"RHI_ENABLE_VULKAN">)
target_compile_definitions(RHI PRIVATE $<$<BOOL:WANLIX_ENABLE_METAL>:"RHI_ENABLE_METAL">)
target_compile_definitions(RHI PRIVATE $<$<BOOL:WANLIX_ENABLE_OPENGL>:"RHI_ENABLE_OPENGL">)

get_target_property(_outdir_ RHI LIBRARY_OUTPUT_DIRECTORY)
message(STATUS "Target: RHI -> ${_outdir_}" )
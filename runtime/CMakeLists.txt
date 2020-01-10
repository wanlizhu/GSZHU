add_library(Wanlix SHARED dllmain.cpp)
SetOutputDirs(Wanlix)

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE _sourceFiles_ "${CSD}/*.cpp" "${CSD}/*.h" "${CSD}/*.impl")
source_group(TREE "${CSD}" FILES ${_sourceFiles_})
target_sources(Wanlix PRIVATE ${_sourceFiles_})

target_include_directories(Wanlix PRIVATE "${CSD}")
target_compile_definitions(Wanlix PRIVATE "_UNICODE" "UNICODE")
target_compile_definitions(Wanlix PRIVATE "WANLIX_BUILD_SHARED_LIBS")
target_compile_definitions(Wanlix PRIVATE $<$<BOOL:WANLIX_ENABLE_D3D12>:"ENABLE_D3D12">)
target_compile_definitions(Wanlix PRIVATE $<$<BOOL:WANLIX_ENABLE_VULKAN>:"ENABLE_VULKAN">)
target_compile_definitions(Wanlix PRIVATE $<$<BOOL:WANLIX_ENABLE_METAL>:"ENABLE_METAL">)
target_compile_definitions(Wanlix PRIVATE $<$<BOOL:WANLIX_ENABLE_OPENGL>:"ENABLE_OPENGL">)
target_link_libraries(Wanlix PUBLIC EXTERNAL_PKG::glm)
target_link_libraries(Wanlix PUBLIC EXTERNAL_PKG::imgui)
target_link_libraries(Wanlix PUBLIC EXTERNAL_PKG::tinygltf)

add_custom_command(
    TARGET Wanlix 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${RSD}/data" "${RBD}/data"
    WORKING_DIRECTORY "${RSD}"
)

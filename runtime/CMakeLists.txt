add_library(Wanlix SHARED dllmain.cpp)
SetOutputDirs(Wanlix)

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE _sourceFiles_ "${CSD}/*.cpp" "${CSD}/*.h" "${CSD}/*.impl")
source_group(TREE "${CSD}" FILES ${_sourceFiles_})
target_sources(Wanlix PRIVATE ${_sourceFiles_})

target_include_directories(Wanlix PRIVATE "${CSD}")
target_compile_definitions(Wanlix PRIVATE "_UNICODE" "UNICODE")
target_compile_definitions(Wanlix PRIVATE "WANLIX_BUILD_SHARED_LIBS")
if (WANLIX_ENABLE_D3D12)
    target_compile_definitions(Wanlix PRIVATE "ENABLE_D3D12")
endif()
if (WANLIX_ENABLE_VULKAN)
    target_compile_definitions(Wanlix PRIVATE "ENABLE_VULKAN")
    find_package(Vulkan REQUIRED)
    target_link_libraries(Wanlix PUBLIC Vulkan::Vulkan)
endif()
target_link_libraries(Wanlix PUBLIC PKG::glm)
target_link_libraries(Wanlix PUBLIC PKG::imgui)
target_link_libraries(Wanlix PUBLIC PKG::tinygltf)
target_link_libraries(Wanlix PRIVATE PKG::glfw)

if (NOT PKG_DLLS)
    set(PKG_DLLS "${RSD}/LICENSE")
endif()
add_custom_command(
    TARGET Wanlix 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${PKG_DLLS}" "${RBD}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${RSD}/data" "${RBD}/data"
    WORKING_DIRECTORY "${RSD}"
)
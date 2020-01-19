add_library(RHI STATIC "${RSD}/dummy.cpp")
SetOutputDirs(RHI)
set_target_properties(RHI PROPERTIES FOLDER "Engine")

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE _interfaceFiles_ 
    "${CSD}/include/RHI/*.h" 
    "${CSD}/include/RHI/*.impl"
)
file(GLOB_RECURSE _sourceFiles_
    "${CSD}/source/RHI/*.cpp" 
    "${CSD}/source/RHI/*.h" 
    "${CSD}/source/RHI/*.impl"
)
source_group(TREE "${CSD}/include" PREFIX "Interface" FILES ${_interfaceFiles_})
source_group(TREE "${CSD}/source" PREFIX "Source Files" FILES ${_sourceFiles_})
target_sources(RHI PRIVATE ${_sourceFiles_} ${_interfaceFiles_})

target_include_directories(RHI PUBLIC "${CSD}/include")
target_include_directories(RHI PRIVATE "${CSD}")
target_include_directories(RHI PRIVATE "${CSD}/source")
target_compile_definitions(RHI PRIVATE "_UNICODE" "UNICODE")

target_link_libraries(RHI PUBLIC PKG::glm)
target_link_libraries(RHI PRIVATE Core)

if (WANLIX_ENABLE_D3D12)
    file(GLOB_RECURSE _interfaceFilesD3D12_ 
    "${CSD}/include/D3D12RHI/*.h" 
    "${CSD}/include/D3D12RHI/*.impl"
    )
    file(GLOB_RECURSE _sourceFilesD3D12_
    "${CSD}/source/D3D12RHI/*.cpp" 
    "${CSD}/source/D3D12RHI/*.h" 
    "${CSD}/source/D3D12RHI/*.impl"
    )
    source_group(TREE "${CSD}/include" PREFIX "Interface" FILES ${_interfaceFilesD3D12_})
    source_group(TREE "${CSD}/source" PREFIX "Source Files" FILES ${_sourceFilesD3D12_})
    target_sources(RHI PRIVATE ${_sourceFilesD3D12_} ${_interfaceFilesD3D12_})
    target_compile_definitions(RHI PRIVATE "ENABLE_D3D12")
endif()

if (WANLIX_ENABLE_VULKAN)
    file(GLOB_RECURSE _interfaceFilesVk_ 
        "${CSD}/include/VulkanRHI/*.h" 
        "${CSD}/include/VulkanRHI/*.impl"
    )
    file(GLOB_RECURSE _sourceFilesVk_
        "${CSD}/source/VulkanRHI/*.cpp" 
        "${CSD}/source/VulkanRHI/*.h" 
        "${CSD}/source/VulkanRHI/*.impl"
    )
    source_group(TREE "${CSD}/include" PREFIX "Interface" FILES ${_interfaceFilesVk_})
    source_group(TREE "${CSD}/source" PREFIX "Source Files" FILES ${_sourceFilesVk_})
    target_sources(RHI PRIVATE ${_sourceFilesVk_} ${_interfaceFilesVk_})
    target_compile_definitions(RHI PRIVATE "ENABLE_VULKAN")
    find_package(Vulkan REQUIRED)
    target_link_libraries(RHI PUBLIC Vulkan::Vulkan)
endif()
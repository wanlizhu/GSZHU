add_subdirectory(thirdparty)

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
set(CBD "${CMAKE_CURRENT_BINARY_DIR}")

add_library(Wanli SHARED "${RSD}/dummy.cpp")
SetOutputDirs(Wanli)
set_target_properties(Wanli PROPERTIES FOLDER "Engine")
find_package(Vulkan)

file(GLOB_RECURSE _headerFiles_ "${CSD}/include/*.h" "${CSD}/include/*.h" "${CSD}/include/*.inl")
file(GLOB_RECURSE _sourceFiles_ "${CSD}/source/*.h" "${CSD}/source/*.h" "${CSD}/source/*.inl" "${CSD}/source/*.cpp")
file(GLOB_RECURSE _shaderFiles_ "${CSD}/shaders/*.glsl")

source_group(TREE "${CSD}/include" PREFIX "Header Files" FILES ${_headerFiles_})
source_group(TREE "${CSD}/source" PREFIX "Source Files" FILES ${_sourceFiles_})
source_group(TREE "${CSD}/shaders" PREFIX "Shader Files" FILES ${_shaderFiles_})

target_sources(Wanli PRIVATE ${_headerFiles_} ${_sourceFiles_} ${_shaderFiles_})
target_include_directories(Wanli PUBLIC "${CSD}/include")
target_include_directories(Wanli PRIVATE "${CSD}/source")
target_compile_definitions(Wanli PRIVATE "_UNICODE" "UNICODE")
target_compile_definitions(Wanli PRIVATE "WANLI_BUILD_SHARED_LIBS")
target_compile_definitions(Wanli PRIVATE $<$<BOOL:WANLI_DEBUG>:"WANLI_DEBUG=1">)
target_compile_definitions(Wanli PRIVATE $<$<BOOL:WANLI_ENABLE_VULKAN>:"ENABLE_VULKAN">)
target_compile_definitions(Wanli PRIVATE $<$<BOOL:WANLI_ENABLE_D3D12>:"ENABLE_D3D12">)
target_link_libraries(Wanli PUBLIC PKG::glm)
target_link_libraries(Wanli PRIVATE PKG::imgui)
target_link_libraries(Wanli PRIVATE PKG::tinygltf)
target_link_libraries(Wanli PRIVATE PKG::physfs)
target_link_libraries(Wanli PRIVATE $<$<BOOL:WANLI_ENABLE_VULKAN>:Vulkan::Vulkan>)

if (WIN32)
    target_compile_definitions(Wanli PRIVATE "PLATFORM_WINDOWS=1")
    target_compile_definitions(Wanli PRIVATE "_CRT_SECURE_NO_WARNINGS")
    target_link_libraries(Wanli PRIVATE PKG::glfw)
    if (WANLI_ENABLE_VULKAN)
        target_compile_definitions(Wanli PRIVATE "VK_USE_PLATFORM_WIN32_KHR")
    endif()
elseif(IOS)
    target_compile_definitions(Wanli PRIVATE "PLATFORM_IOS=1")
elseif(APPLE)
    target_compile_definitions(Wanli PRIVATE "PLATFORM_MAC=1")
elseif(UNIX)
    target_compile_definitions(Wanli PRIVATE "PLATFORM_LINUX=1")
endif()

add_custom_command(
    TARGET Wanli 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${PKG_BINS}" "${CSD}/config.json" "${CBD}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${RSD}/data" "${CBD}/data"
    WORKING_DIRECTORY "${RSD}"
)
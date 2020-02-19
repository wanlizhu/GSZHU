add_subdirectory(thirdparty)

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
set(CBD "${CMAKE_CURRENT_BINARY_DIR}")

add_library(Wanli SHARED "${RSD}/dummy.cpp")
SetOutputDirs(Wanli)
set_target_properties(Wanli PROPERTIES FOLDER "Engine")
find_package(Vulkan)

file(GLOB_RECURSE _sourceFiles_ "${CSD}/source/*.h" "${CSD}/source/*.h" "${CSD}/source/*.inl" "${CSD}/source/*.cpp")
file(GLOB_RECURSE _shaderFiles_ "${CSD}/shaders/*.glsl")

source_group(TREE "${CSD}/source" PREFIX "Source Files" FILES ${_sourceFiles_})
source_group(TREE "${CSD}/shaders" PREFIX "Shader Files" FILES ${_shaderFiles_})

target_sources(Wanli PRIVATE ${_sourceFiles_} ${_shaderFiles_})
target_include_directories(Wanli PUBLIC "${CSD}/source")
target_include_directories(Wanli PRIVATE "${CSD}/thirdparty")
target_compile_definitions(Wanli PRIVATE "_UNICODE" "UNICODE")
target_compile_definitions(Wanli PRIVATE "USE_PREDEFINED_MAIN")
target_compile_definitions(Wanli PRIVATE "ENABLE_ASYNC_LOG")

target_link_libraries(Wanli PUBLIC PKG::glm)
target_link_libraries(Wanli PRIVATE PKG::imgui)
target_link_libraries(Wanli PRIVATE PKG::tinygltf)
target_link_libraries(Wanli PRIVATE PKG::LodePNG)

if (WANLI_BUILD_SHARED_LIB)
    target_compile_definitions(Wanli PRIVATE "WANLI_BUILD_SHARED_LIB")
else()
    target_compile_definitions(Wanli PRIVATE "WANLI_STATIC_LIB")
endif()

if (WANLI_DEBUG)
    target_compile_definitions(Wanli PRIVATE "WANLI_DEBUG=1")
endif()

if (WANLI_ENABLE_VULKAN)
    target_compile_definitions(Wanli PRIVATE "ENABLE_VULKAN")
    target_link_libraries(Wanli PRIVATE Vulkan::Vulkan)
endif()

if (WIN32)
    target_compile_definitions(Wanli PRIVATE "PLATFORM_WINDOWS=1")
    target_compile_definitions(Wanli PRIVATE "WIN32_LEAN_AND_MEAN")
    target_compile_definitions(Wanli PRIVATE "_SCL_SECURE_NO_WARNINGS")
    target_compile_definitions(Wanli PRIVATE "_CRT_SECURE_NO_WARNINGS")
    target_compile_definitions(Wanli PRIVATE "_WINSOCK_DEPRECATED_NO_WARNINGS")
    target_compile_definitions(Wanli PRIVATE "NOMINMAX")

    target_compile_options(Wanli PRIVATE "/wd4251") # members of exported class are not exported
    target_compile_options(Wanli PRIVATE "/wd4996") # when using std::codecvt_utf8_utf16<wchar_t>

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
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CSD}/assets" "${CBD}/assets"
    WORKING_DIRECTORY "${RSD}"
)
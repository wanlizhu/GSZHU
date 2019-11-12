add_library(WanlixCoreGS STATIC "${RSD}/dummy.cpp")

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE _headers_    "${CSD}/include/*.h" "${CSD}/include/*.hpp")
file(GLOB_RECURSE _sources_    "${CSD}/source/*.c"  "${CSD}/source/*.cpp")
source_group(TREE "${CSD}/include"  PREFIX "Header Files" FILES ${_headers_})
source_group(TREE "${CSD}/source"   PREFIX "Source Files" FILES ${_sources_})
target_sources(WanlixCoreGS PRIVATE ${_headers_} ${_sources_})

target_include_directories(WanlixCoreGS PUBLIC "${CSD}/include" PRIVATE "${CSD}/source")
target_link_libraries(WanlixCoreGS PRIVATE CONAN_PKG::glfw)
target_link_libraries(WanlixCoreGS PRIVATE CONAN_PKG::imgui)

# Enable OpenGL
find_package(OpenGL)
if (OPENGL_FOUND)
    target_link_libraries(WanlixCoreGS PRIVATE OpenGL::GL)
endif()

# Enable DX12
if (GE_ENABLE_DX12 AND WIN32)
    target_link_libraries(WanlixCoreGS PRIVATE "d3d12.lib")
endif()

# Enable Vulkan
if (GE_ENABLE_VULKAN)
    find_package(Vulkan REQUIRED)
    target_link_libraries(WanlixCoreGS PRIVATE Vulkan::Vulkan)
endif()

get_target_property(_outdir_ WanlixCoreGS LIBRARY_OUTPUT_DIRECTORY)
message(STATUS "Target: WanlixCoreGS -> ${_outdir_}" )
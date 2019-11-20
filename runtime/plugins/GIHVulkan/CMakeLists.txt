add_library(HWGI STATIC "${RSD}/dummy.cpp")
if (NOT GE_OUTPUT_WITH_CONFIG_SUBDIR)
    get_target_property(_outdir_ HWGI RUNTIME_OUTPUT_DIRECTORY)
    get_target_property(_libdir_ HWGI LIBRARY_OUTPUT_DIRECTORY)
    set_target_properties(HWGI PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${_outdir_}")
    set_target_properties(HWGI PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${_outdir_}")
    set_target_properties(HWGI PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINISIZEREL    "${_outdir_}")
    set_target_properties(HWGI PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${_outdir_}")
    set_target_properties(HWGI PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${_libdir_}")
    set_target_properties(HWGI PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${_libdir_}")
    set_target_properties(HWGI PROPERTIES LIBRARY_OUTPUT_DIRECTORY_MINISIZEREL    "${_libdir_}")
    set_target_properties(HWGI PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${_libdir_}")
endif()

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE _headers_    "${CSD}/include/*.h" "${CSD}/include/*.hpp")
file(GLOB_RECURSE _sources_    "${CSD}/source/*.c"  "${CSD}/source/*.cpp")
source_group(TREE "${CSD}/include"  PREFIX "Header Files" FILES ${_headers_})
source_group(TREE "${CSD}/source"   PREFIX "Source Files" FILES ${_sources_})
target_sources(HWGI PRIVATE ${_headers_} ${_sources_})

target_include_directories(HWGI PUBLIC "${CSD}/include")
target_include_directories(HWGI PRIVATE "${CSD}/source")
target_include_directories(HWGI PRIVATE "${CSD}")
target_link_libraries(HWGI PRIVATE CONAN_PKG::glfw)
target_link_libraries(HWGI PRIVATE CONAN_PKG::imgui)
target_link_libraries(HWGI PRIVATE CONAN_PKG::Assimp)
target_link_libraries(HWGI PRIVATE CONAN_PKG::jsonformoderncpp)
target_link_libraries(HWGI PRIVATE CONAN_PKG::stb)

# Enable OpenGL
find_package(OpenGL)
if (OPENGL_FOUND)
    target_link_libraries(HWGI PRIVATE OpenGL::GL)
endif()

# Enable DX12
if (GE_ENABLE_DX12 AND WIN32)
    target_link_libraries(HWGI PRIVATE "d3d12.lib")
endif()

# Enable Vulkan
if (GE_ENABLE_VULKAN)
    find_package(Vulkan REQUIRED)
    target_link_libraries(HWGI PRIVATE Vulkan::Vulkan)
endif()

get_target_property(_outdir_ HWGI LIBRARY_OUTPUT_DIRECTORY)
message(STATUS "Target: HWGI -> ${_outdir_}" )
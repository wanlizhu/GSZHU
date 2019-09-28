cmake_minimum_required(VERSION 3.1)

option(RHI_ENABLE_VULKAN "Enable Vulkan backend" OFF)
option(RHI_ENABLE_D3D12  "Enable D3D12 backend"  OFF)

find_package(GLOG REQUIRED)

zhu_add_shared_library(FabricRHI_OpenGL)
    zhu_set_target_folder("Plugins")
    zhu_set_arch(x64)
    zhu_set_charset(UNICODE)
    zhu_set_cxx_standard(17)

    zhu_add_source_files("source/dllmain.cpp" "source/OpenGL" "include" "interface")
    zhu_include_directories("source/OpenGL" "include")
    zhu_include_directories_public("interface")
    zhu_compile_definitions(RHI_USE_OPENGL RHI_BUILD_SHARED_LIBS)

    find_package(OpenGL REQUIRED)
    zhu_link_libraries(FabricGML OpenGL GLOG)
zhu_end_shared_library()


if (RHI_ENABLE_VULKAN)
    zhu_add_shared_library(FabricRHI_Vulkan)
        zhu_set_target_folder("Plugins")
        zhu_set_arch(x64)
        zhu_set_charset(UNICODE)
        zhu_set_cxx_standard(17)

        zhu_add_source_files("source/dllmain.cpp" "source/Vulkan" "include" "interface")
        zhu_include_directories("source/Vulkan" "include")
        zhu_include_directories_public("interface")
        zhu_compile_definitions(RHI_USE_VULKAN RHI_BUILD_SHARED_LIBS)

        find_package(vulkan REQUIRED)
        zhu_link_libraries(FabricGML Vulkan GLOG)
    zhu_end_shared_library()
endif()

if (RHI_ENABLE_D3D12)
    zhu_add_shared_library(FabricRHI_D3D12)
        zhu_set_target_folder("Plugins")
        zhu_set_arch(x64)
        zhu_set_charset(UNICODE)
        zhu_set_cxx_standard(17)

        zhu_add_source_files("source/dllmain.cpp" "source/D3D12" "include" "interface")
        zhu_include_directories("source/D3D12" "include")
        zhu_include_directories_public("interface")
        zhu_compile_definitions(RHI_USE_D3D12 RHI_BUILD_SHARED_LIBS)

        zhu_link_libraries(FabricGML GLOG)
    zhu_end_shared_library()
endif()

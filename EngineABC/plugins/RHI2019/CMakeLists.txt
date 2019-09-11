cmake_minimum_required(VERSION 2.8.11)

option(RHI_ENABLE_VULKAN "Enable Vulkan backend" ON)
option(RHI_ENABLE_D3D12  "Enable D3D12 backend"  ON)


zhu_add_shared_library(RHI2019_OpenGL)
zhu_set_target_version(1, 0)
zhu_set_target_folder("Plugins")
zhu_set_arch(x64)
zhu_set_charset(UNICODE)
zhu_set_cxx_standard(17)

zhu_add_source_files("source/dllmain.cpp" "source/OpenGL" "include" "interface")
zhu_include_directories("source/OpenGL" "include")
zhu_include_directories_public("interface")
zhu_compile_definitions(RHI_USE_OPENGL)

find_package(OpenGL REQUIRED)
zhu_link_libraries(OpenGL
                   External::EIGEN
                   External::GLFW
                   External::GLOG
                   External::STB)
zhu_end_shared_library()


if (RHI_ENABLE_VULKAN)
    find_package(Vulkan REQUIRED)

    zhu_add_shared_library(RHI2019_Vulkan)
    zhu_set_target_version(1, 0)
    zhu_set_target_folder("Plugins")
    zhu_set_arch(x64)
    zhu_set_charset(UNICODE)
    zhu_set_cxx_standard(17)

    zhu_add_source_files("source/dllmain.cpp" "source/Vulkan" "include" "interface")
    zhu_include_directories("source/Vulkan" "include")
    zhu_include_directories_public("interface")
    zhu_compile_definitions(RHI_USE_VULKAN)

    zhu_link_libraries(Vulkan::Vulkan
                       External::EIGEN
                       External::GLFW
                       External::GLOG
                       External::STB)
    zhu_end_shared_library()
endif()

if (RHI_ENABLE_D3D12)
    zhu_add_shared_library(RHI2019_D3D12)
    zhu_set_target_version(1, 0)
    zhu_set_target_folder("Plugins")
    zhu_set_arch(x64)
    zhu_set_charset(UNICODE)
    zhu_set_cxx_standard(17)

    zhu_add_source_files("source/dllmain.cpp" "source/D3D12" "include" "interface")
    zhu_include_directories("source/D3D12" "include")
    zhu_include_directories_public("interface")
    zhu_compile_definitions(RHI_USE_D3D12)

    zhu_link_libraries(External::EIGEN
                       External::GLFW
                       External::GLOG
                       External::STB)
    zhu_end_shared_library()
endif()

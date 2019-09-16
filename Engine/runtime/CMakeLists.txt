cmake_minimum_required(VERSION 3.1)

zhu_add_shared_library(GraphicsEngine)
    zhu_set_arch(x64)
    zhu_set_charset(UNICODE)
    zhu_set_cxx_standard(17)

    zhu_add_source_files("source" "include")
    zhu_include_directories("source")
    zhu_include_directories_public("include")

    find_package(glfw3)
    find_package(glog)
    zhu_link_libraries(FabricRHI_OpenGL
                       FabricGML
                       glfw3
                       glog)
    zhu_add_resources("data")
zhu_end_shared_library()

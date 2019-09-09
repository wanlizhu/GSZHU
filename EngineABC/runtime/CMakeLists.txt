cmake_minimum_required(VERSION 2.8.11)

zhu_add_shared_library(EngineABC)
    zhu_set_target_version(1, 0)
    zhu_set_arch(x64)
    zhu_set_charset(UNICODE)
    zhu_set_cxx_standard(17)

    zhu_add_source_files("source" "include")
    zhu_include_directories("source")
    zhu_include_directories_public("include")

    zhu_link_libraries(RHI2019_OpenGL
                       External::EIGEN
                       External::GLFW
                       External::GLOG
                       External::STB)
    zhu_copy_to_bin("../data")
zhu_end_shared_library()

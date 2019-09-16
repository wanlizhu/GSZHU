cmake_minimum_required(VERSION 3.1)

zhu_add_executable(GraphicsEngine_UnitTest)
    zhu_set_arch(x64)
    zhu_set_charset(UNICODE)
    zhu_set_cxx_standard(17)

    zhu_add_source_files("source")
    zhu_include_directories("source")

    find_package(gtest)
    zhu_link_libraries(GraphicsEngine gtest)
    zhu_add_resources("data")
zhu_end_executable()
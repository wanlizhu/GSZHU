cmake_minimum_required(VERSION 2.8.11)

zhu_add_executable(EngineABC_UnitTest)
    zhu_set_target_version(1, 0)
    zhu_set_arch(x64)
    zhu_set_charset(UNICODE)
    zhu_set_cxx_standard(17)

    zhu_add_source_files("source")
    zhu_include_directories("source")

    zhu_link_libraries(EngineABC
                       External::GTEST)
    zhu_copy_to_bin("data")
zhu_end_executable()
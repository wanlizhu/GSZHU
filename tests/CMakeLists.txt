cmake_minimum_required(VERSION 3.11)

cm_add_executable(UnitTestGE2)
cm_set_arch(x64)
cm_set_charset(UNICODE)
cm_set_cxx_standard(17)

cm_add_source_files("source")
cm_include_directories("source")

cm_compile_definitions("_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS")

cm_link_libraries(GE2)

cm_use_gtest()
cm_additional_library_directories("${GE_PACKAGES_DIR}")

cm_end_executable()
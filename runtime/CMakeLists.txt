cmake_minimum_required(VERSION 3.1)

cm_add_shared_library(GE2)
cm_set_output_name(DEBUG GE2d RELEASE GE2)
cm_set_arch(x64)
cm_set_charset(UNICODE)
cm_set_cxx_standard(17)

cm_add_source_files("source" "include")
cm_include_directories(PUBLIC "include" PRIVATE "source")

cm_use_eigen3()
cm_use_nlohmann_json()
cm_use_tinygltf()
cm_use_glfw3()
cm_use_imgui()
cm_use_tinyobjloader()

cm_end_shared_library()
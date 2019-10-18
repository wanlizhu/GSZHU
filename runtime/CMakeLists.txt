cmake_minimum_required(VERSION 3.11)

cm_add_shared_library(GE2)
cm_set_output_name(DEBUG GE2d RELEASE GE2)
cm_set_arch(x64)
cm_set_charset(UNICODE)
cm_set_cxx_standard(17)

cm_add_source_files("source" "include")
cm_include_directories(PUBLIC "include" PRIVATE "source")

cm_compile_definitions("GE_BUILD_SHARED_LIBS")
cm_compile_definitions("_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS")
cm_disable_specific_warnings(4251)

cm_use_eigen3()
cm_use_nlohmann_json()
cm_use_tinygltf()
cm_use_glfw3()
cm_use_imgui()
cm_use_tinyobjloader()
cm_use_vulkan()
cm_additional_library_directories("${GE_PACKAGES_DIR}")

cm_end_shared_library()
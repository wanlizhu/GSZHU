cmake_minimum_required(VERSION 3.1)

zhu_add_shared_library(FabricGML)
    zhu_set_target_folder("Plugins")
    zhu_set_arch(x64)
    zhu_set_charset(UNICODE)
    zhu_set_cxx_standard(17)
    
    zhu_add_source_files("source" "include")
    zhu_include_directories("source")
    zhu_include_directories_public("include")
    zhu_compile_definitions_if(GML_USE_CGAL)
    
    #find_package(CGAL REQUIRED)
    #zhu_link_libraries(CGAL)
zhu_end_shared_library()
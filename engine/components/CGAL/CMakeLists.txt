add_library(CGAL STATIC "${RSD}/dummy.cpp")
SetOutputDirs(CGAL)
set_target_properties(CGAL PROPERTIES FOLDER "Engine")

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE _interfaceFiles_ 
    "${CSD}/include/*.h" 
    "${CSD}/include/*.impl"
)
file(GLOB_RECURSE _sourceFiles_
    "${CSD}/source/*.cpp" 
    "${CSD}/source/*.h" 
    "${CSD}/source/*.impl"
)
source_group(TREE "${CSD}/include" PREFIX "Interface" FILES ${_interfaceFiles_})
source_group(TREE "${CSD}/source" PREFIX "Source Files" FILES ${_sourceFiles_})
target_sources(CGAL PRIVATE ${_sourceFiles_} ${_interfaceFiles_})

target_include_directories(CGAL PUBLIC "${CSD}/include")
target_include_directories(CGAL PRIVATE "${WANLIX_INTERFACE_DIR}")
target_include_directories(CGAL PRIVATE "${CSD}")
target_include_directories(CGAL PRIVATE "${CSD}/source")
target_compile_definitions(CGAL PRIVATE "_UNICODE" "UNICODE")

target_link_libraries(CGAL PUBLIC PKG::glm)
target_link_libraries(CGAL PRIVATE Core)
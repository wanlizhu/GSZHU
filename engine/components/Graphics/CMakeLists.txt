add_library(Graphics STATIC "${RSD}/dummy.cpp")
SetOutputDirs(Graphics)
set_target_properties(Graphics PROPERTIES FOLDER "Engine")

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
target_sources(Graphics PRIVATE ${_sourceFiles_} ${_interfaceFiles_})

target_include_directories(Graphics PUBLIC "${CSD}/include")
target_include_directories(Graphics PRIVATE "${WANLIX_INTERFACE_DIR}")
target_include_directories(Graphics PRIVATE "${CSD}")
target_include_directories(Graphics PRIVATE "${CSD}/source")
target_compile_definitions(Graphics PRIVATE "_UNICODE" "UNICODE")

target_link_libraries(Graphics PUBLIC PKG::glm)
target_link_libraries(Graphics PUBLIC PKG::imgui)
target_link_libraries(Graphics PUBLIC PKG::tinygltf)
target_link_libraries(Graphics PRIVATE PKG::glfw)
target_link_libraries(Graphics PRIVATE PKG::json)
target_link_libraries(Graphics PRIVATE Core)
target_link_libraries(Graphics PRIVATE RHI)

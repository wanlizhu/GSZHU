add_library(Core STATIC "${RSD}/dummy.cpp")
SetOutputDirs(Core)
set_target_properties(Core PROPERTIES FOLDER "Engine")

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
target_sources(Core PRIVATE ${_sourceFiles_} ${_interfaceFiles_})

target_include_directories(Core PUBLIC "${CSD}/include")
target_include_directories(Core PRIVATE "${WANLIX_INTERFACE_DIR}")
target_include_directories(Core PRIVATE "${CSD}")
target_include_directories(Core PRIVATE "${CSD}/source")
target_compile_definitions(Core PUBLIC "ENABLE_LOG")
target_compile_definitions(Core PRIVATE "_UNICODE" "UNICODE")

target_link_libraries(Core PUBLIC PKG::glm)
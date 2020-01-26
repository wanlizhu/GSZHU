set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
set(CBD "${CMAKE_CURRENT_BINARY_DIR}")
set(WANLIX_INTERFACE_DIR "${CSD}/include")

add_subdirectory(thirdparty)
add_subdirectory(components)

add_library(Wanlix SHARED "${CSD}/source/dllmain.cpp")
SetOutputDirs(Wanlix)
set_target_properties(Wanlix PROPERTIES FOLDER "Engine")

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
target_sources(Wanlix PRIVATE ${_sourceFiles_} ${_interfaceFiles_})

target_include_directories(Wanlix PUBLIC "${WANLIX_INTERFACE_DIR}")
target_include_directories(Wanlix PRIVATE "${CSD}")
target_include_directories(Wanlix PRIVATE "${CSD}/source")
target_compile_definitions(Wanlix PRIVATE "_UNICODE" "UNICODE")
target_compile_definitions(Wanlix PRIVATE "WANLIX_BUILD_SHARED_LIBS")

target_link_libraries(Wanlix PUBLIC PKG::glm)
target_link_libraries(Wanlix PRIVATE Graphics)
target_link_libraries(Wanlix PRIVATE CGAL)

if (NOT PKG_DLLS)
    set(PKG_DLLS "${RSD}/LICENSE")
endif()
add_custom_command(
    TARGET Wanlix 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${PKG_DLLS}" "${RBD}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${RSD}/data" "${RBD}/data"
    WORKING_DIRECTORY "${RSD}"
)
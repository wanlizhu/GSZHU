add_subdirectory(Core)
add_subdirectory(RHI)
add_subdirectory(Graphics)
add_subdirectory(CGAL)

add_library(Wanlix SHARED dllmain.cpp)
SetOutputDirs(Wanlix)
set_target_properties(Wanlix PROPERTIES FOLDER "Engine")

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(Wanlix PRIVATE "${CSD}")
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
set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
set(PKG_BINS "" PARENT_SCOPE)

# glm
add_library(PKG::glm INTERFACE IMPORTED GLOBAL)
target_include_directories(PKG::glm INTERFACE glm)

# json
add_library(PKG::json INTERFACE IMPORTED GLOBAL)
target_include_directories(PKG::json INTERFACE nlohmann)

# imgui
add_library(PKG::imgui STATIC IMPORTED GLOBAL
    imgui/imconfig.h
    imgui/imgui.h
    imgui/imgui_internal.h
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
)
target_include_directories(PKG::imgui INTERFACE imgui)
set_target_properties(PKG::imgui PROPERTIES FOLDER "Externals")

# tinygltf
add_library(PKG::tinygltf INTERFACE IMPORTED GLOBAL)
target_include_directories(PKG::tinygltf INTERFACE tinygltf)

# GLFW
add_library(PKG::glfw SHARED IMPORTED GLOBAL)
target_include_directories(PKG::glfw INTERFACE "${CSD}/glfw/include")
if (WIN32)
    set_target_properties(PKG::glfw PROPERTIES IMPORTED_LOCATION "${CSD}/glfw/lib-vc2019/glfw3.dll")
    set_target_properties(PKG::glfw PROPERTIES IMPORTED_IMPLIB "${CSD}/glfw/lib-vc2019/glfw3dll.lib")
    list(APPEND PKG_BINS "${CSD}/glfw/lib-vc2019/glfw3.dll")
endif()
if (APPLE)
    list(APPEND PKG_BINS "${CSD}/glfw/lib-macos/libglfw.3.dylib")
endif()
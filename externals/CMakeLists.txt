set(EXTERNAL_PKG_DLLS PARENT_SCOPE)

# glm
add_library(glm INTERFACE)
add_library(EXTERNAL_PKG::glm ALIAS glm)
target_include_directories(glm INTERFACE glm)

# imgui
add_library(imgui STATIC 
    imgui/imconfig.h
    imgui/imgui.h
    imgui/imgui_internal.h
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
)
add_library(EXTERNAL_PKG::imgui ALIAS imgui)
target_include_directories(imgui PUBLIC imgui)
set_target_properties(imgui PROPERTIES FOLDER "Externals")

# tinygltf
add_library(tinygltf INTERFACE)
add_library(EXTERNAL_PKG::tinygltf ALIAS tinygltf)
target_include_directories(tinygltf INTERFACE tinygltf)

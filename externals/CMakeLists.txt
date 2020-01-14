set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
set(PKG_DLLS "" PARENT_SCOPE)

# glm
add_library(glm INTERFACE)
add_library(PKG::glm ALIAS glm)
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
add_library(PKG::imgui ALIAS imgui)
target_include_directories(imgui PUBLIC imgui)
set_target_properties(imgui PROPERTIES FOLDER "Externals")

# tinygltf
add_library(tinygltf INTERFACE)
add_library(PKG::tinygltf ALIAS tinygltf)
target_include_directories(tinygltf INTERFACE tinygltf)

# GLFW
add_library(glfw SHARED IMPORTED GLOBAL)
add_library(PKG::glfw ALIAS glfw)
target_include_directories(glfw INTERFACE "${CSD}/glfw/include")
if (WIN32)
    set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "${CSD}/glfw/lib-vc2019/glfw3.dll")
    set_target_properties(glfw PROPERTIES IMPORTED_IMPLIB "${CSD}/glfw/lib-vc2019/glfw3dll.lib")
    list(APPEND PKG_DLLS "${CSD}/glfw/lib-vc2019/glfw3.dll")
endif()
if (APPLE)
    list(APPEND PKG_DLLS "${CSD}/glfw/lib-macos/libglfw.3.dylib")
endif()
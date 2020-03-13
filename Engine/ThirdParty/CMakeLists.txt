set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
set(PKG_BINS "" PARENT_SCOPE)

# glm
add_library(PKG::glm INTERFACE IMPORTED GLOBAL)
target_include_directories(PKG::glm INTERFACE glm)

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
set_target_properties(imgui PROPERTIES FOLDER "ThirdParty")

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

# LodePNG
add_library(LodePNG STATIC "LodePNG/lodepng.cpp" "LodePNG/lodepng.h")
add_library(PKG::LodePNG ALIAS LodePNG)
set_target_properties(LodePNG PROPERTIES FOLDER "ThirdParty")
target_include_directories(LodePNG PUBLIC "LodePNG")
target_compile_options(LodePNG PRIVATE "/wd4551") # function call missing argument list
target_compile_options(LodePNG PRIVATE "/wd4334") # '<<': result of 32-bit shift implicitly converted to 64 bits 
target_compile_options(LodePNG PRIVATE "/wd4267") # '=': conversion from 'size_t' to 'unsigned short'

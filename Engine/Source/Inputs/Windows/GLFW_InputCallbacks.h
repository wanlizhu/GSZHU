#pragma once
#ifndef __ANDROID__
#include <stdint.h>
#include "Window/Windows/GLFW_Types.h"

struct GLFWwindow;

namespace Wanli
{
    void WANLI_API OnKey_GLFW(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
    void WANLI_API OnChar_GLFW(GLFWwindow* window, uint32_t codepoint);
    void WANLI_API OnMouseButton_GLFW(GLFWwindow* window, int32_t button, int32_t action, int32_t mods);
    void WANLI_API OnMouseScroll_GLFW(GLFWwindow* window, double xoffset, double yoffset);
    void WANLI_API OnCursorPos_GLFW(GLFWwindow* window, double xpos, double ypos);
    void WANLI_API OnCursorEnter_GLFW(GLFWwindow* window, int32_t entered);
    void WANLI_API OnDrop_GLFW(GLFWwindow* window, int count, const char** paths);
}

#endif
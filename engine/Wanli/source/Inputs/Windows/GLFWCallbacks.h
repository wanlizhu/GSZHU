#pragma once

#include "Inputs/InputFlags.h"

struct GLFWwindow;

namespace Wanli
{
    void OnKey_GLFW(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
    void OnChar_GLFW(GLFWwindow* window, uint32_t codepoint);
    void OnMouseButton_GLFW(GLFWwindow* window, int32_t button, int32_t action, int32_t mods);
    void OnMouseScroll_GLFW(GLFWwindow* window, double xoffset, double yoffset);
    void OnCursorPos_GLFW(GLFWwindow* window, double xpos, double ypos);
    void OnCursorEnter_GLFW(GLFWwindow* window, int32_t entered);
}
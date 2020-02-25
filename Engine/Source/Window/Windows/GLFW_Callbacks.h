#pragma once
#ifdef PLATFORM_WINDOWS
#include <stdint.h>

struct GLFWwindow;

namespace Wanli
{
    void OnError_GLFW(int32_t error, const char* description);
    void OnWindowPos_GLFW(GLFWwindow* window, int32_t xpos, int32_t ypos);
    void OnWindowSize_GLFW(GLFWwindow* window, int32_t width, int32_t height);
    void OnWindowClose_GLFW(GLFWwindow* window);
    void OnWindowIconified_GLFW(GLFWwindow* window, int32_t iconified);
}
#endif
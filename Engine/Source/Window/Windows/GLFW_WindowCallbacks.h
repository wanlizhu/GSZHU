#pragma once
#ifndef __ANDROID__
#include <stdint.h>
#include "GLFW_Types.h"

struct GLFWwindow;

namespace Wanli
{
    void WANLI_API OnError_GLFW(int32_t error, const char* description);
    void WANLI_API OnWindowPos_GLFW(GLFWwindow* window, int32_t xpos, int32_t ypos);
    void WANLI_API OnWindowSize_GLFW(GLFWwindow* window, int32_t width, int32_t height);
    void WANLI_API OnWindowClose_GLFW(GLFWwindow* window);
    void WANLI_API OnWindowIconified_GLFW(GLFWwindow* window, int32_t iconified);
}
#endif
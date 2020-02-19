#include "Inputs/Windows/GLFWCallbacks.h"
#include "Inputs/Windows/GLFWEnumConv.h"
#include "Events/Event.h"
#include "GLFW/glfw3.h"

namespace Wanli
{
    void OnKey_GLFW(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
    {
        Event::Create(
            EVT_Keyboard,
            GlfwToEKey(key),
            GlfwToEInputAction(action),
            GlfwToEInputMods(mods))->PostEvent();
    }

    void OnChar_GLFW(GLFWwindow* window, uint32_t codepoint)
    {
        Event::Create(EVT_KeyboardChar, codepoint)->PostEvent();
    }

    void OnMouseButton_GLFW(GLFWwindow* window, int32_t button, int32_t action, int32_t mods)
    {
        Event::Create(
            EVT_MouseButton,
            GlfwToEMouseButton(button), 
            GlfwToEInputAction(action),
            GlfwToEInputMods(mods))->PostEvent();
    }

    void OnMouseScroll_GLFW(GLFWwindow* window, double xoffset, double yoffset)
    {
        Event::Create(EVT_MouseWheel, xoffset, yoffset)->PostEvent();
    }

    void OnCursorPos_GLFW(GLFWwindow* window, double xpos, double ypos)
    {
        Event::Create(EVT_MouseMove, xpos, ypos)->PostEvent();
    }

    void OnCursorEnter_GLFW(GLFWwindow* window, int32_t entered)
    {
        Event::Create(EVT_MouseEnter, entered)->PostEvent();
    }
}
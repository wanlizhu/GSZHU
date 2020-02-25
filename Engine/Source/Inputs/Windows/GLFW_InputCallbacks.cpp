#include "Core/Common.h"
#include "GLFW_InputCallbacks.h"
#include "Window/Windows/GLFW_Types.h"
#include "Events/Windows/KeyboardEvent.h"
#include "Events/Windows/MouseEvent.h"
#include "GLFW/glfw3.h"

namespace Wanli
{
    void OnKey_GLFW(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
    {
        if (action == GLFW_PRESS)
        {
            EventPool::Allocate<KeyDownEvent>(
                (EKey)key,
                KeyMods(mods))->PostEvent();
        }
        else if (action == GLFW_RELEASE)
        {
            EventPool::Allocate<KeyUpEvent>(
                (EKey)key,
                KeyMods(mods))->PostEvent();
        }
    }

    void OnChar_GLFW(GLFWwindow* window, uint32_t codepoint)
    {}

    void OnMouseButton_GLFW(GLFWwindow* window, int32_t button, int32_t action, int32_t mods)
    {
        if (action == GLFW_PRESS)
        {
            EventPool::Allocate<MouseButtonDownEvent>(
                (EMouseButton)button,
                KeyMods(mods))->PostEvent();
        }
        else if (action == GLFW_RELEASE)
        {
            EventPool::Allocate<MouseButtonUpEvent>(
                (EMouseButton)button,
                KeyMods(mods))->PostEvent();
        }
    }

    void OnMouseScroll_GLFW(GLFWwindow* window, double xoffset, double yoffset)
    {
        EventPool::Allocate<MouseScrollEvent>(
            glm::vec2((float)xoffset, (float)yoffset))->PostEvent();
    }

    void OnCursorPos_GLFW(GLFWwindow* window, double xpos, double ypos)
    {
        EventPool::Allocate<MouseMoveEvent>(
            glm::ivec2((int)xpos, (int)ypos))->PostEvent();
    }

    void OnCursorEnter_GLFW(GLFWwindow* window, int32_t entered)
    {
        if (entered)
        {
            EventPool::Allocate<MouseEnterEvent>()->PostEvent();
        }
        else 
        {
            EventPool::Allocate<MouseLeaveEvent>()->PostEvent();
        }
    }

    void OnDrop_GLFW(GLFWwindow* window, int count, const char** paths)
    {
        StringArray strs(count);
        for (int i = 0; i < count; i++)
        {
            strs.push_back(paths[i]);
        }

        EventPool::Allocate<MouseFileDroppedEvent>(strs)->PostEvent();
    }
}
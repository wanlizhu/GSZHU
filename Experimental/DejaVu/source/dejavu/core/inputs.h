#pragma once

#include <string>
#include <vector>
#include <memory>

namespace djv
{
    struct KeyboardEvent
    {
        int key = 0;
        int scancode = 0;
        int action = 0;
        int mods = 0;

        KeyboardEvent() = default;
        KeyboardEvent(int key, int scancode, int action, int mods);
    };

    struct MouseButtonAction
    {
        int button = 0;
        int action = 0;
        int mods = 0;
    };

    struct MOUSE_MOVE_TAG {};

    struct MouseMoveAction
    {
        double x = 0.0;
        double y = 0.0;
    };

    struct MOUSE_SCROLL_TAG {};

    struct MouseScrollAction
    {
        double x = 0.0;
        double y = 0.0;
    };

    struct MouseDropAction
    {
        int count = 0;
        const char** paths = nullptr;
    };

    struct MouseEvent
    {
        std::optional<MouseButtonAction> buttonAction;
        std::optional<MouseMoveAction>   moveAction;
        std::optional<MouseScrollAction> scrollAction;
        std::optional<MouseDropAction>   dropAction;

        MouseEvent() = default;
        MouseEvent(int button, int action, int mods);
        MouseEvent(double x, double y, MOUSE_MOVE_TAG);
        MouseEvent(double x, double y, MOUSE_SCROLL_TAG);
        MouseEvent(int count, const char* paths[]);
    };
}
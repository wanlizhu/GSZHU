#include "inputs.h"

namespace djv
{
    KeyboardEvent::KeyboardEvent(int key_, int scancode_, int action_, int mods_)
        : key(key_)
        , scancode(scancode_)
        , action(action_)
        , mods(mods_)
    {}

    MouseEvent::MouseEvent(int button, int action, int mods)
        : buttonAction(MouseButtonAction())
    {
        buttonAction.value().button = button;
        buttonAction.value().action = action;
        buttonAction.value().mods = mods;
    }

    MouseEvent::MouseEvent(double x, double y, MOUSE_MOVE_TAG)
        : moveAction(MouseMoveAction())
    {
        moveAction.value().x = x;
        moveAction.value().y = y;
    }

    MouseEvent::MouseEvent(double x, double y, MOUSE_SCROLL_TAG)
        : scrollAction(MouseScrollAction())
    {
        scrollAction.value().x = x;
        scrollAction.value().y = y;
    }

    MouseEvent::MouseEvent(int count, const char* paths[])
        : dropAction(MouseDropAction())
    {
        dropAction.value().count = count;
        dropAction.value().paths = paths;
    }
}
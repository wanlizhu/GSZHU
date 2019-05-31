#pragma once

#include <GSZHU/Config.h>
#include <GSZHU/IOEvents.h>

namespace GSZHU {
    class IUserInput {
    public:
        virtual void OnKey(SKeyEvent keyEvent) = 0;
        virtual void OnMouseButton(SMouseButtonEvent buttonEvent) = 0;
        virtual void OnMouseMotion(SMouseMotionEvent motionEvent) = 0;
        virtual void OnMouseWheel(SMouseWheelEvent wheelEvent) = 0;
    };
}
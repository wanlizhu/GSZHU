#pragma once

#include <GSZHU/Config.h>
#include <GSZHU/IOEvents.h>

namespace GSZHU {
    class IUserInput {
    public:
        virtual void OnKey(std::shared_ptr<SKeyEvent> keyEvent) = 0;
        virtual void OnMouseButton(std::shared_ptr<SMouseButtonEvent> buttonEvent) = 0;
        virtual void OnMouseMotion(std::shared_ptr<SMouseMotionEvent> motionEvent) = 0;
        virtual void OnMouseWheel(std::shared_ptr<SMouseWheelEvent> wheelEvent) = 0;
    };
}
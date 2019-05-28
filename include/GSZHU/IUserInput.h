#pragma once

#include <GSZHU/CManagedAPI.h>

namespace GSZHU {
    class IUserInput : public CManagedAPI<IUserInput> {
    public:
        virtual void OnKeyDown() = 0;
        virtual void OnKeyUp() = 0;
        virtual void OnMouseButtonDown() = 0;
        virtual void OnMouseButtonUp() = 0;
        virtual void OnMouseMove() = 0;
        virtual void OnMouseWheel() = 0;
        virtual void OnWindowResize() = 0;
        virtual void OnWindowDestroy() = 0;
    };
}
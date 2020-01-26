#pragma once

#include "Core/Common.h"

namespace Wanlix
{
    class IRenderer
    {
    public:
        virtual void OnInitialize(IDrawContext* dc) = 0;
        virtual void OnDraw(IDrawContext* dc) = 0;
        virtual void OnDestroy() = 0;
        virtual void OnResize(Uint width, Uint height) = 0;
        virtual void OnLoadData() = 0;
        virtual bool OnKeyEvent(const KeyEvent& event) = 0;
        virtual bool OnMouseEvent(const MouseEvent& event) = 0;
        virtual void OnDroppedFile(const String& filename) = 0;
    };
}
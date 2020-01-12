#pragma once

#include "Core/Common.h"
#include "Utils/UI/UserInput.h"

namespace Wanlix
{
    class DLL_DECL IRenderer
    {
    public:
        IRenderer() = default;
        IRenderer(const IRenderer&) = delete;
        IRenderer& operator=(const IRenderer&) = delete;
        virtual ~IRenderer() {}

        virtual void OnInitialize(DrawContext* dc) = 0;
        virtual void OnDraw(DrawContext* dc) = 0;
        virtual void OnDrawUI(UiData* ui) = 0;
        virtual void OnDestroy() = 0;
        virtual void OnResize(Uint width, Uint height) = 0;
        virtual void OnLoadData() = 0;
        virtual bool OnKeyEvent(const KeyEvent& event) = 0;
        virtual bool OnMouseEvent(const MouseEvent& event) = 0;
        virtual void OnDroppedFile(const String& filename) = 0;
    };
}
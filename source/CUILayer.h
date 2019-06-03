#pragma once

#include <GSZHU/Config.h>
#include <GSZHU/IWindow.h>
#include <imgui/imgui.h>
#include "CNonCopyable.h"

namespace GSZHU {
    class CUILayer : public CNonCopyable
                   , public std::enable_shared_from_this<CUILayer> {
    public:
        void Initialize(std::weak_ptr<IWindow> window);
        void Destroy();
        void NewFrame();
        void Render();

    private:
        std::weak_ptr<IWindow> mWindow;
        ImGuiContext* mImGuiContext = nullptr;
    };
}
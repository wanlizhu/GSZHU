#include "Wanlix/Core3D/Platform/IWindow.h"

namespace Wanlix
{
    IWindow::IWindow()
    {}

    SurfaceType IWindow::GetSurfaceType() const
    {
        return SurfaceType::DesktopWindow;
    }

    bool IWindow::AdaptForVideoMode(const VideoModeDescriptor& videoModeDesc)
    {
        /* Query current window descriptor */
        auto windowDesc = GetDescriptor();

        /* Adapt window size and position */
        windowDesc.size = videoModeDesc.resolution;

        if (videoModeDesc.fullscreen)
        {
            windowDesc.borderless = true;
            windowDesc.position = { 0, 0 };
        }
        else
        {
            windowDesc.borderless = false;
            windowDesc.centered = true;
        }

        /* Set new window descriptor and return with success */
        SetDescriptor(windowDesc);

        return true;
    }

    bool IWindow::Tick()
    {
        OnProcessEvents(*this);
        ProcessEventsInternal();
        return !mQuitFlag;
    }

    IDisplay::UniquePtr IWindow::GetResidentDisplay() const
    {
        auto displayList = IDisplay::List();

        const auto winPos = GetPosition();
        const auto winSize = GetContentSize();
        const auto winArea = static_cast<int>(winSize.width * winSize.height);

        for (auto& display : displayList)
        {
            auto offset = display->GetOffset();
            auto extent = display->GetDisplayMode().resolution;

            int scrX = static_cast<int>(extent.width);
            int scrY = static_cast<int>(extent.height);

            /* Calculate window boundaries relative to the current display */
            int x1 = winPos.x - offset.x;
            int y1 = winPos.y - offset.y;
            int x2 = x1 + static_cast<int>(winSize.width);
            int y2 = y1 + static_cast<int>(winSize.height);

            /* Is window fully or partially inside the dispaly? */
            if (x2 >= 0 && x1 <= scrX &&
                y2 >= 0 && y1 <= scrY)
            {
                /* Is at least the half of the window inside the display? */
                x1 = std::max(0, x1);
                y1 = std::max(0, y1);

                x2 = std::min(x2 - x1, scrX);
                y2 = std::min(y2 - y1, scrY);

                auto visArea = x2 * y2;

                if (visArea * 2 >= winArea) {
                    return std::move(display);
                }
            }
        }

        return nullptr;
    }
}
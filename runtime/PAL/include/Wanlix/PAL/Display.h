#pragma once

#include "Common.h"

namespace Wanlix
{
    struct VideoMode
    {
        INT2 resolution;
        bool fullscreen = false;
        int  colorBits = 32;
        int  depthBits = 24;
        int  stencilBits = 8;
        int  swapChainSize = 2;
    };

    struct DisplayMode
    {
        INT2 resolution;
        int  refreshRate = 0;
    };

    class Display
    {
    public:
        using SharedPtr = std::shared_ptr<Display>;
        using UniquePtr = std::unique_ptr<Display>;

        static std::vector<UniquePtr> List();
        static UniquePtr Primary();
        static void ShowCursor();
        static void HideCursor();
        static bool IsCursorVisible();

        virtual std::wstring GetDeviceName() const = 0;
        virtual bool         IsPrimary() const = 0;
        virtual INT2         GetOffset() const = 0;

        virtual bool ResetDisplayMode() = 0;
        virtual bool SetDisplayMode(const DisplayMode& mode) = 0;

        virtual DisplayMode              GetDisplayMode() const = 0;
        virtual std::vector<DisplayMode> GetSupportedDisplayMode() const = 0;
    
    protected:
        void SortDisplayModes(std::vector<DisplayMode>& modes) const;
    };
}
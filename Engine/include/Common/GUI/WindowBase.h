#pragma once

#include "Common/Config.h"
#include <string>

namespace ZHU
{
    class ZHU_API WindowBase
    {
    public:
        struct ZHU_API CreationInfo
        {
            PCCH title = "Untitled";
            INT2 pos{ 0, 0 };
            INT2 size{ 1000, 618 };

            // Window related hints
            bool  resizable = true;
            bool    visible = true;
            bool  decorated = true;
            bool fullscreen = false;

            // Framebuffer related hints
            struct 
            {
                struct 
                {
                    int   red = 8;
                    int green = 8;
                    int  blue = 8;
                    int alpha = 8;
                } color;
                
                int    depth = 24;
                int  stencil = 8;
                int  samples = 0;
                bool    SRGB = false;
                int  refresh = 0;
            } framebuffer;

            // Context related hints
            struct 
            {
                struct 
                {
                    int          major = 1;
                    int          minor = 0;
                    bool forwardCompat = false;
                    bool         debug = false;
                    int        profile = 0; // o for ANY, 1 for COMPAT, 2 for CORE
                } openGL;
            } context;
        };

        virtual void SetTitle(PCCH title) = 0;
        virtual void SetFullScreen(bool enabled = true) = 0;
        virtual void SetSize(int width, int height) = 0;
        virtual void MoveTo(int x, int y) = 0;
        virtual void MainLoop() = 0;
        virtual void Hide() = 0;
        virtual void Show() = 0;
        virtual void Destroy() = 0;

        virtual PCCH GetTitle() const = 0;
        virtual INT2 GetPosition() const = 0;
        virtual INT2 GetSize() const = 0;
    };
}
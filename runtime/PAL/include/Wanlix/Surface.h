#pragma once

#include <memory>
#include <string>
#include "Display.h"

namespace Wanlix
{
    class Surface
    {
    public:
        using SharedPtr = std::shared_ptr<Surface>;
        using UniquePtr = std::unique_ptr<Surface>;

        virtual void* GetHandle() const = 0;
        virtual INT2  GetContentSize() const = 0;
        virtual bool  ProcessEvents() const = 0;
        virtual bool  AdaptForVideoMode(const VideoMode& videoMode) = 0;
        virtual Display::UniquePtr GetResidentDisplay() const = 0;
    };
}
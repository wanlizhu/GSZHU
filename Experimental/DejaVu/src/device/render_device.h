#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "dejavu/core/preprocess.h"

namespace djv
{
    class IRenderDevice
    {
    public:
        virtual ~IRenderDevice() = default;
        virtual void waitIdle() const = 0;
        virtual bool setupSwapchain(void* window, int imageCount) = 0;
    };

    DJV_API IRenderDevice* create_render_device();
    DJV_API void delete_render_device(IRenderDevice* device);
}
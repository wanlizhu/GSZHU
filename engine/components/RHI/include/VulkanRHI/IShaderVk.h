#pragma once

#include "RHI/IShader.h"
#include "VulkanRHI/VkUtilities.h"
#include "VulkanRHI/IDeviceVk.h"

namespace Wanlix
{
    class IShaderVk : public IShader
    {
    public:
        virtual SharedPtr<ByteArray> GetSPIRV() const = 0;
    };
}
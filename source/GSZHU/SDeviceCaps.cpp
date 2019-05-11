#pragma once

#include <GSZHU/SDeviceCaps.h>

namespace GSZHU {
    float SDeviceCaps::NDCAttribs::GetZtoDepthBias() const {
        // Returns ZtoDepthBias such that given NDC z coordinate, depth value can be
        // computed as follows:
        // d = z * ZtoDepthScale + ZtoDepthBias
        return -MinZ * ZtoDepthScale;
    }

    bool SDeviceCaps::IsGLDevice() const {
        return DeviceType == EDeviceType::OpenGL || DeviceType == EDeviceType::OpenGLES;
    }

    bool SDeviceCaps::IsD3DDevice() const {
        return DeviceType == EDeviceType::D3D11 || DeviceType == EDeviceType::D3D12;
    }

    bool SDeviceCaps::IsVulkanDevice() const {
        return DeviceType == EDeviceType::Vulkan;
    }

    const SDeviceCaps::NDCAttribs& SDeviceCaps::GetNDCAttribs() const {
        if (IsVulkanDevice()) {
            // Note that Vulkan itself does not invert Y coordinate when transforming normalized device Y to window space.
            // However, we use negative viewport height which achieves the same effect as in D3D, 
            // thererfore we need to invert y 
            static constexpr const NDCAttribs NDCAttribsVk{ 0.0f, 1.0f, -0.5f };
            return NDCAttribsVk;
        }
        else if (IsD3DDevice()) {
            static constexpr const NDCAttribs NDCAttribsD3D{ 0.0f, 1.0f, -0.5f };
            return NDCAttribsD3D;
        }
        else if (IsGLDevice()) {
            static constexpr const NDCAttribs NDCAttribsGL{ -1.0f, 0.5f, 0.5f };
            return NDCAttribsGL;
        }
        else {
            static constexpr const NDCAttribs NDCAttribsDefault{ 0.0f, 1.0f, 0.5f };
            return NDCAttribsDefault;
        }
    }
}
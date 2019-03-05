#pragma once

#include <NTGS/Graphics/GraphicsEnums.h>

namespace NTGS {
    enum class DeviceType : INT {
        Undefined = 0,  
        D3D11,          
        D3D12,          
        OpenGL,         
        OpenGLES,       
        Vulkan,         
        Metal           
    };

    struct SamplerCaps {
        bool BorderSamplingModeSupported = true;
        bool AnisotropicFilteringSupported = true;
        bool LODBiasSupported = true;
    };

    struct TextureCaps {
        bool Texture1DSupported = true;
        bool Texture1DArraySupported = true;
        bool Texture2DMSSupported = true;
        bool Texture2DMSArraySupported = true;
        bool TextureViewSupported = true;
        bool CubemapArraysSupported = true;
    };

    struct DeviceCaps {
        struct NDCAttribs {
            const FLOAT MinZ;
            const FLOAT ZToDepthScale;
            const FLOAT YToVScale;   

            // d = z * ZtoDepthScale + ZtoDepthBias
            FLOAT GetZToDepthBias() const { return -MinZ * ZToDepthScale; }
        };

        DeviceType DevType = DeviceType::Undefined;
        INT MajorVersion = 0;
        INT MinorVersion = 0;
        bool SeparableProgramSupported = true;
        bool IndirectRenderingSupported = true;
        bool WireframeFillSupported = true;
        bool MultithreadedResourceCreationSupported = false;
        bool ComputeShadersSupported = true;
        bool GeometryShadersSupported = true;
        bool TessellationSupported = true;
        SamplerCaps SamCaps;
        TextureCaps TexCaps;

        bool IsGLDevice() const { return DevType == DeviceType::OpenGL || DevType == DeviceType::OpenGLES; }
        bool IsD3DDevice() const { return DevType == DeviceType::D3D11 || DevType == DeviceType::D3D12; }
        bool IsVulkanDevice() const { return DevType == DeviceType::Vulkan;}

        const NDCAttribs& GetNDCAttribs() const {
            if (IsVulkanDevice()) {
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
    };
}
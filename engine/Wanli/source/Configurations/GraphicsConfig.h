#pragma once

#include "Configurations/IConfig.h"

namespace Wanli
{
    class GraphicsModule;

    enum class ERHIDevice
    {
        Vulkan = 0,
        D3D12,
    };

    class DLLDECL GraphicsConfig : public IConfig
    {
        friend class GraphicsModule;
    public:
        using Module = GraphicsModule;

        explicit GraphicsConfig(const Configuration& configManager)
            : IConfig(configManager)
        {}

        GraphicsConfig& SetDeviceType(ERHIDevice deviceType);
        GraphicsConfig& SetWindowHandle(WindowHandle windowHandle);
        GraphicsConfig& AddEnabledExtensions(const PCCHArray& extensions);
        GraphicsConfig& AddEnabledLayers(const PCCHArray& layers);
        
        GraphicsConfig& SetBackgroundColor(const Color& color);
        GraphicsConfig& SetBackbufferCount(Uint count);

        GraphicsConfig& EnableMultiThreading(bool value);
        GraphicsConfig& EnableDebug(bool value);
        GraphicsConfig& EnableUI(bool value);
        GraphicsConfig& EnableGPURaytracing(bool value);
        GraphicsConfig& EnableMeshShader(bool value);

    protected:
        ERHIDevice mDeviceType = ERHIDevice::Vulkan;
        WindowHandle mWindowHandle = 0;
        PCCHArray mEnabledExtensions = {};
        PCCHArray mEnabledLayers = {};

        Color mBackgroundColor = Color_AutoCAD;
        Uint mBackbufferCount = 2;

        bool mEnableMultiThreading = true;
        bool mEnableDebug = true;
        bool mEnableUI = true;
        bool mEnableGPURayTracing = true;
        bool mEnableMeshShader = true;
    };
}
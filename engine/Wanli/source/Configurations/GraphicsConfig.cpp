#include "Configurations/GraphicsConfig.h"

namespace Wanli
{
    GraphicsConfig& GraphicsConfig::SetDeviceType(ERHIDevice deviceType)
    {
        AssertWritable();
        mDeviceType = deviceType;
        return *this;
    }

    GraphicsConfig& GraphicsConfig::SetWindowHandle(WindowHandle windowHandle)
    {
        AssertWritable();
        mWindowHandle = windowHandle;
        return *this;
    }

    GraphicsConfig& GraphicsConfig::AddEnabledExtensions(const PCCHArray& extensions)
    {
        AssertWritable();
        mEnabledExtensions.insert(mEnabledExtensions.end(),
            extensions.begin(),
            extensions.end());
        return *this;
    }

    GraphicsConfig& GraphicsConfig::AddEnabledLayers(const PCCHArray& layers)
    {
        AssertWritable();
        mEnabledLayers.insert(mEnabledLayers.end(),
            layers.begin(),
            layers.end());
        return *this;
    }

    GraphicsConfig& GraphicsConfig::SetBackgroundColor(const Color& color)
    {
        AssertWritable();
        mBackgroundColor = color;
        return *this;
    }

    GraphicsConfig& GraphicsConfig::SetBackbufferCount(Uint count)
    {
        AssertWritable();
        mBackbufferCount = count;
        return *this;
    }

    GraphicsConfig& GraphicsConfig::EnableMultiThreading(bool value)
    {
        AssertWritable();
        mEnableMultiThreading = value;
        return *this;
    }

    GraphicsConfig& GraphicsConfig::EnableDebug(bool value)
    {
        AssertWritable();
        mEnableDebug = value;
        return *this;
    }

    GraphicsConfig& GraphicsConfig::EnableUI(bool value)
    {
        AssertWritable();
        mEnableUI = value;
        return *this;
    }

    GraphicsConfig& GraphicsConfig::EnableGPURaytracing(bool value)
    {
        AssertWritable();
        mEnableGPURayTracing = value;
        return *this;
    }

    GraphicsConfig& GraphicsConfig::EnableMeshShader(bool value)
    {
        AssertWritable();
        mEnableMeshShader = value;
        return *this;
    }
}
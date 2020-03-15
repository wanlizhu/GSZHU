#pragma once

#include "AcadDM-Vulkan-App-Window.h"
#include "GIDeviceVk.h"
#include "GISwapchainVk.h"

namespace AGE = AutoCAD::Graphics::Engine;

class AppDelegate
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    void SetWindow(Window* window);
    void LoadData();

private:
    Window* mWindow = nullptr;
    AGE::SharedPtr<AGE::GIDeviceVk> mDevice;
    AGE::SharedPtr<AGE::GISwapchainVk> mSwapchain;
};
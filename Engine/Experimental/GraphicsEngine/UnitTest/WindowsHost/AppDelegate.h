#pragma once

#include "Window.h"
#include "GIDeviceVk.h"
#include "GISwapchainVk.h"
#include "GIGraphicsPipelineVk.h"

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
    AGE::SharedPtr<AGE::GIGraphicsPipelineVk> mGraphicsPipeline;
};
#include "AppDelegate.h"

using namespace AutoCAD::Graphics::Engine;

AppDelegate::AppDelegate()
{}

AppDelegate::~AppDelegate()
{}

void AppDelegate::SetWindow(Window* window)
{
    mWindow = window;

    auto instance = VKInstanceBuilder()
        .SetApiVersion(VK_API_VERSION_1_2)
        .EnableValidationLayers(true)
        .EnableLayers(GIPlatformVk::RequiredInstanceLayers())
        .EnableExtensions(GIPlatformVk::RequiredInstanceExtensions())
        .Build();

    mDevice = GIDeviceBuilderVk(instance)
        .SetPhysicalDevice(instance->FindPhysicalDeviceBy({}, {}, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT, {}))
        .EnableExtensions(GIPlatformVk::RequiredDeviceExtensions(instance->GetChosenGPUVendorID()))
        .EnableFeatures(GIPlatformVk::RequiredDeviceFeatures())
        .CreateGraphicsQueue()
        .CreateTransferQueue()
        .Build();

    mSwapchain = GISwapchainBuilderVk(mDevice)
        .SetWindow(window->GetHwnd())
        .AcquireNextImageAsync(true)
        .EnableVsync(true)
        .SetBackbufferCount(3)
        .SetColorFormat(VK_FORMAT_B8G8R8A8_UNORM)
        .Build();


}

void AppDelegate::LoadData()
{}
#include "DeviceVulkan.h"

namespace GE2::RHI
{
    DeviceVulkan::~DeviceVulkan()
    {
        Destroy();
    }

    bool DeviceVulkan::Initialize()
    {
        try
        {
            CreateInstance();
            EnableLayers();
            EnumeratePhysicalDevices();
            CreateLogicalDevices();
            CreateSwapChain();
            CreateSwapChainImageViews();
        }
        catch (const std::runtime_error& err)
        {
            printf("Error @RHI (failed to initialize vulkan device):%s\n", err.what());
            return false;
        }
        return true;
    }

    void DeviceVulkan::Destroy()
    {

    }

    void DeviceVulkan::CreateInstance()
    {

    }

    void DeviceVulkan::EnableLayers()
    {

    }

    void DeviceVulkan::EnumeratePhysicalDevices()
    {

    }

    void DeviceVulkan::CreateLogicalDevices()
    {

    }

    void DeviceVulkan::CreateSwapChain()
    {

    }

    void DeviceVulkan::CreateSwapChainImageViews()
    {

    }
}
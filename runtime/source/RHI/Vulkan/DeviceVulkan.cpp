#include "DeviceVulkan.h"

namespace GE2::RHI
{
    DeviceVulkan::~DeviceVulkan()
    {
        Destroy();
    }

    bool DeviceVulkan::Initialize(const InitializeData& data)
    {
        try
        {
            CreateInstance(data.instanceExtensions);
            EnableSpecificLayers(data.enableLayers);
            EnumeratePhysicalDevices(data.deviceExtensions);
            CreateLogicalDevices();
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

    void DeviceVulkan::CreateInstance(const STRLIST& extensions)
    {

    }

    void DeviceVulkan::EnableSpecificLayers(const STRLIST& layers)
    {

    }

    void DeviceVulkan::EnumeratePhysicalDevices(const STRLIST& extensions)
    {

    }

    void DeviceVulkan::CreateLogicalDevices()
    {

    }
}
#pragma once

#include "GE2/RHI/IDevice.h"
#include "VulkanCommon.h"

namespace GE2::RHI
{
    enum class EQueueFamily
    {
        Graphics = 0,
        Compute = 1,
        Transfer = 2,
    };

    class VulkanDevice final : public IDevice
    {
    public:
        VulkanDevice() = default;
        virtual ~VulkanDevice();

        // IDeviceObject's abstract virtual methods
        virtual bool   Initialize(const InitializeData& data) override;
        bool           Initialize(VkInstance existedInstance, 
                                  VkPhysicalDevice physicalDevice,
                                  const InitializeData& data);
        virtual void   Destroy() override;
        virtual CSTR   GetName() const override;
        virtual HANDLE GetHandle() const override;

        // IDevice's abstract virtual methods


    private:
        STR                               mName = "Unnamed";
        VkInstance                        mVkInstance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT          mVkDebugUtilsMessengerHandle = VK_NULL_HANDLE;
        VkPhysicalDevice                  mVkPhysicalDevice = VK_NULL_HANDLE; // will be implicitly destroyed when the VkInstance is destroyed
        MAP<EQueueFamily, LIST<VkQueue>>  mQueues;
        VkDevice                          mVkDevice = VK_NULL_HANDLE;
    };
}
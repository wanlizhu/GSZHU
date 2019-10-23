#pragma once

#include "GE2/RHI/IDevice.h"
#include "VulkanCommon.h"

namespace GE2::RHI
{
    struct QueueFamilies
    {
        LIST<VkQueueFamilyProperties> __queueFamilyProperties;
        std::optional<uint32_t> graphicsFamilyIndex;
        std::optional<uint32_t> computeFamilyIndex;
        std::optional<uint32_t> transferFamilyIndex;

        inline bool empty() const noexcept
        {
            return graphicsFamilyIndex.has_value() ||
                   computeFamilyIndex.has_value()  ||
                   transferFamilyIndex.has_value();
        }

        inline bool isCapable() const 
        {
            return graphicsFamilyIndex.has_value();
        }
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
                                  QueueFamilies queueFamilies,
                                  const InitializeData& data);
        virtual void   Destroy() override;
        virtual CSTR   GetName() const override;
        virtual HANDLE GetHandle() const override;

        // IDevice's abstract virtual methods


    private:
        bool                      mIsExternalInstance = false;
        VkInstance                mVkInstance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT  mVkDebugUtilsMessengerHandle = VK_NULL_HANDLE;
        VkPhysicalDevice          mVkPhysicalDevice = VK_NULL_HANDLE; 

        STR                       mName = "Unnamed";
        VkDevice                  mVkDevice = VK_NULL_HANDLE;
        LIST<VkQueue>             mVkGraphicsQueues;
        LIST<VkQueue>             mVkComputeQueues;
        LIST<VkQueue>             mVkTransferQueues;
    };
}
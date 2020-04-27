#include "render_device_imp_vk.h"

namespace djv
{
    RenderDevice_Vulkan::RenderDevice_Vulkan(
        const std::vector<const char>& validationLayers,
        const std::vector<const char>& instanceExtensions,
        const std::vector<const char>& deviceExtensions
    ) 
        : IRenderDevice()
    {

    }

    RenderDevice_Vulkan::~RenderDevice_Vulkan()
    {}

    VkPhysicalDevice RenderDevice_Vulkan::getPhysicalDevice() const
    {
        return mPhysicalDevice;
    }

    VkDevice RenderDevice_Vulkan::getLogicalDevice() const
    {
        return mLogicalDevice;
    }

    VkQueue RenderDevice_Vulkan::getQueue(uint32_t familyIndex) const
    {
        auto found = mQueueHandleMap.find(familyIndex);
        return found == mQueueHandleMap.end() ? nullptr : found->second;
    }

    uint32_t RenderDevice_Vulkan::getQueueFamilyIndex(VkQueueFlagBits flags) const
    {
        using value = typename std::unordered_map<QueueIndex, VkQueueFlagBits>::value_type;
        auto found = std::find_if(mQueueFlagMap.begin(), mQueueFlagMap.end(),
                                  [&](const value& it) {
                                      return (it.second & flags) != 0;
                                  });
        return found == mQueueFlagMap.end() ? UINT_MAX : found->first;
    }

    uint32_t RenderDevice_Vulkan::getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlagBits props) const
    {
        return 0;
    }

    bool RenderDevice_Vulkan::isValid() const
    {
        return true;
    }

    void RenderDevice_Vulkan::waitIdle() const
    {}

    bool RenderDevice_Vulkan::setupPresentQueue(VkSurfaceKHR surface)
    {
        return true;
    }
}
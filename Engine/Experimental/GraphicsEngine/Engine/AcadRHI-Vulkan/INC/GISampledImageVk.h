#pragma once

#include "GIImageVk.h"
#include "GISamplerVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GISampledImageVk : public GIImageVk
    {
    public:
        static SharedPtr<GISampledImageVk> Create(
            SharedPtr<GIDeviceVk> device,
            VkImageType imageType,
            VkFormat format,
            VkExtent2D extent,
            VkImageLayout imageLayout,
            VkSampleCountFlagBits samples,
            VkImageUsageFlagBits usage,
            std::vector<uint32_t> sharingQueues,
            VkMemoryPropertyFlags properties,
            const void* initialData,
            EResourceState initialState,
            SharedPtr<GISamplerVk> sampler
        );

        virtual ~GISampledImageVk();
        bool IsValid() const;

        void SetSampler(SharedPtr<GISamplerVk> sampler);
        SharedPtr<GISamplerVk> GetSampler() const;

    protected:
        GISampledImageVk(SharedPtr<GIImageVk> device);

    private:
        SharedPtr<GISamplerVk> mSampler;
    };
}
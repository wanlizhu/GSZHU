#pragma once

#include "GIImageVk.h"
#include "GISamplerVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GISampledImageVk : public std::enable_shared_from_this<GISampledImageVk>
    {
    public:
        static SharedPtr<GISampledImageVk> Create(SharedPtr<GIImageVk> image, SharedPtr<GISamplerVk> sampler);

        virtual ~GISampledImageVk();
        operator const VkImage& () const;
        bool IsValid() const;

        void SetImage(SharedPtr<GIImageVk> image);
        void SetSampler(SharedPtr<GISamplerVk> sampler);

        SharedPtr<GIImageVk> GetImage() const;
        SharedPtr<GISamplerVk> GetSampler() const;

    protected:
        GISampledImageVk(
            SharedPtr<GIImageVk> image,
            SharedPtr<GISamplerVk> sampler
        );

    private:
        SharedPtr<GIImageVk> mImage;
        SharedPtr<GISamplerVk> mSampler;
    };
}
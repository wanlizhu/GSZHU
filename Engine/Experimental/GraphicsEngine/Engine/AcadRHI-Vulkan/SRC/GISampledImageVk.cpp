#include "GISampledImageVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GISampledImageVk> GISampledImageVk::Create(
        SharedPtr<GIImageVk> image,
        SharedPtr<GISamplerVk> sampler
    )
    {
        return SharedPtr<GISampledImageVk>(new GISampledImageVk(image, sampler));
    }

    GISampledImageVk::GISampledImageVk(
            SharedPtr<GIImageVk> image,
            SharedPtr<GISamplerVk> sampler
    )
        : mImage(image)
        , mSampler(sampler)
    {}

    GISampledImageVk::~GISampledImageVk()
    {}

    GISampledImageVk::operator const VkImage& () const
    {
        return (VkImage)(*mImage);
    }

    bool GISampledImageVk::IsValid() const
    {
        return mImage->IsValid() && mSampler->IsValid();
    }

    void GISampledImageVk::SetImage(SharedPtr<GIImageVk> image)
    {
        mImage = image;
    }

    void GISampledImageVk::SetSampler(SharedPtr<GISamplerVk> sampler)
    {
        mSampler = sampler;
    }

    SharedPtr<GIImageVk> GISampledImageVk::GetImage() const
    {
        return mImage;
    }

    SharedPtr<GISamplerVk> GISampledImageVk::GetSampler() const
    {
        return mSampler;
    }
}
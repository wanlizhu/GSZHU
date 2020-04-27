#include "GISamplerVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GISamplerVk::GISamplerVk(
        SharedPtr<GIDeviceVk> device,
        const VkSamplerCreateInfo& createInfo
    )
        : GIDeviceObjectVk(device)
        , mSamplerInfo(createInfo)
    {}

    GISamplerVk::~GISamplerVk()
    {
        if (IsValid())
        {
            vkDestroySampler(*mDevice, mSamplerHandle, nullptr);
            mSamplerHandle = VK_NULL_HANDLE;
        }
    }

    bool GISamplerVk::IsValid() const
    {
        return mSamplerHandle != VK_NULL_HANDLE;
    }

    void GISamplerVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mSamplerHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT,
            name
        );
    }

    void GISamplerVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mSamplerHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT,
            tag
        );
    }

    GISamplerVk::operator const VkSampler& () const
    {
        return mSamplerHandle;
    }

    VkSamplerCreateInfo const& GISamplerVk::GetSamplerInfo() const
    {
        return mSamplerInfo;
    }

    GISamplerBuilderVk::GISamplerBuilderVk(SharedPtr<GIDeviceVk> device)
        : mDevice(device)
    {
        mCreateInfo = {};
        mCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        mCreateInfo.pNext = nullptr;
        mCreateInfo.flags = 0;
        mCreateInfo.magFilter = VK_FILTER_LINEAR;
        mCreateInfo.minFilter = VK_FILTER_LINEAR;
        mCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        mCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        mCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        mCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        mCreateInfo.mipLodBias = 0.0f;
        mCreateInfo.anisotropyEnable = VK_FALSE;
        mCreateInfo.maxAnisotropy = 1.0f;
        mCreateInfo.compareEnable = VK_FALSE;
        mCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        mCreateInfo.minLod = 0.0f;
        mCreateInfo.maxLod = 1.0f;
        mCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
        mCreateInfo.unnormalizedCoordinates = VK_FALSE;
    }

    GISamplerBuilderVk& GISamplerBuilderVk::SetMagFilter(VkFilter filter)
    {
        mCreateInfo.magFilter = filter;
        return *this;
    }

    GISamplerBuilderVk& GISamplerBuilderVk::SetMinFilter(VkFilter filter)
    {
        mCreateInfo.minFilter = filter;
        return *this;
    }

    GISamplerBuilderVk& GISamplerBuilderVk::SetMipmapMode(VkSamplerMipmapMode mode)
    {
        mCreateInfo.mipmapMode = mode;
        return *this;
    }

    GISamplerBuilderVk& GISamplerBuilderVk::SetAddressModeU(VkSamplerAddressMode addressU)
    {
        mCreateInfo.addressModeU = addressU;
        return *this;
    }

    GISamplerBuilderVk& GISamplerBuilderVk::SetAddressModeV(VkSamplerAddressMode addressV)
    {
        mCreateInfo.addressModeU = addressV;
        return *this;
    }

    GISamplerBuilderVk& GISamplerBuilderVk::SetAddressModeW(VkSamplerAddressMode addressW)
    {
        mCreateInfo.addressModeW = addressW;
        return *this;
    }

    GISamplerBuilderVk& GISamplerBuilderVk::SetMipLodBias(float mipLodBias)
    {
        mCreateInfo.mipLodBias = mipLodBias;
        return *this;
    }

    GISamplerBuilderVk& GISamplerBuilderVk::SetAnisotropy(bool enable, float maxAnisotropy)
    {
        mCreateInfo.anisotropyEnable = enable ? VK_TRUE : VK_FALSE;
        mCreateInfo.maxAnisotropy = maxAnisotropy;
        return *this;
    }

    GISamplerBuilderVk& GISamplerBuilderVk::SetCompareOp(bool enable, VkCompareOp compareOp)
    {
        mCreateInfo.compareEnable = enable ? VK_TRUE : VK_FALSE;
        mCreateInfo.compareOp = compareOp;
        return *this;
    }

    GISamplerBuilderVk& GISamplerBuilderVk::SetLod(float minLod, float maxLod)
    {
        mCreateInfo.minLod = minLod;
        mCreateInfo.maxLod = maxLod;
        return *this;
    }

    GISamplerBuilderVk& GISamplerBuilderVk::SetBorderColor(VkBorderColor color)
    {
        mCreateInfo.borderColor = color;
        return *this;
    }

    GISamplerBuilderVk& GISamplerBuilderVk::SetUnnormalizedCoordinates(bool value)
    {
        mCreateInfo.unnormalizedCoordinates = value ? VK_TRUE : VK_FALSE;
        return *this;
    }

    SharedPtr<GISamplerVk> GISamplerBuilderVk::Build()
    {
        auto result = SharedPtr<GISamplerVk>(new GISamplerVk(mDevice, mCreateInfo));
        assert(result->IsValid());
        return result;
    }
}
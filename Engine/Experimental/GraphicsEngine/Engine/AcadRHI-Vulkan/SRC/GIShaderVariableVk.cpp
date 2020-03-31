#include "GIShaderVariableVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIShaderVariableVk::GIShaderVariableVk(uint32_t location, LOCATION_TAG)
        : mLocation(location)
    {}

    GIShaderVariableVk::GIShaderVariableVk(uint32_t constId, CONST_ID_TAG)
        : mConstId(constId)
    {}

    GIShaderVariableVk::GIShaderVariableVk(uint32_t inputAttachment, INPUT_ATTACHMENT_ATG)
        : mInputAttachmentId(inputAttachment)
    {}

    GIShaderVariableVk::GIShaderVariableVk(SET_ID set, BINDING_ID binding)
        : mBinding(GIDescriptorBindingVk(set, binding))
    {}

    std::optional<uint32_t> GIShaderVariableVk::GetLocation() const
    {
        return mLocation;
    }

    std::optional<uint32_t> GIShaderVariableVk::GetConstId() const
    {
        return mConstId;
    }

    std::optional<uint32_t> GIShaderVariableVk::GetInputAttachmentId() const
    {
        return mInputAttachmentId;
    }

    std::optional<GIDescriptorBindingVk> GIShaderVariableVk::GetBinding() const
    {
        return mBinding;
    }

    const std::string& GIShaderVariableVk::GetName() const
    {
        return mName;
    }

    bool GIShaderVariableVk::IsWritable() const
    {
        return mWritable;
    }

    uint32_t GIShaderVariableVk::GetSizeInBytes() const
    {
        return mSizeInBytes;
    }

    VkShaderStageFlags GIShaderVariableVk::GetShaderStages() const
    {
        return mShaderStages;
    }

    VkFormat GIShaderVariableVk::GetFormat() const
    {
        return mFormat;
    }

    uint32_t GIShaderVariableVk::GetArrayLength() const
    {
        return mArrayLength;
    }

    std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> const& GIShaderVariableVk::GetBlockFields() const
    {
        return mBlockFields;
    }
}
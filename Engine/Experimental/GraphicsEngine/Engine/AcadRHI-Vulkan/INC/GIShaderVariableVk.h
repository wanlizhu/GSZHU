#pragma once

#include "GIDescriptorBindingVk.h"

namespace AutoCAD::Graphics::Engine
{
    struct LOCATION_TAG {};
    struct CONST_ID_TAG {};
    struct INPUT_ATTACHMENT_ATG {};

    class GIShaderVariableVk
    {
    public:
        GIShaderVariableVk(uint32_t location, LOCATION_TAG);
        GIShaderVariableVk(uint32_t constId, CONST_ID_TAG);
        GIShaderVariableVk(uint32_t inputAttachment, INPUT_ATTACHMENT_ATG);
        GIShaderVariableVk(SET_ID set, BINDING_ID binding);

        std::optional<uint32_t> GetLocation() const;
        std::optional<uint32_t> GetConstId() const;
        std::optional<uint32_t> GetInputAttachmentId() const;
        std::optional<GIDescriptorBindingVk> GetBinding() const;

        const std::string& GetName() const;
        bool IsWritable() const;
        uint32_t GetSizeInBytes() const;
        VkShaderStageFlags GetShaderStages() const;
        VkFormat GetFormat() const; // VK_FORMAT_UNDEFINED for block and array type
        uint32_t GetArrayLength() const; // 1 for non-array type
        std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> const& GetBlockFields() const;

    private:
        std::optional<uint32_t> mLocation;               // For stage input/output variable
        std::optional<uint32_t> mConstId;                // For specialization variable
        std::optional<uint32_t> mInputAttachmentId;      // For input attachment used in fragment stage
        std::optional<GIDescriptorBindingVk> mBinding;   // For descriptor binding variable
        
        std::string mName;
        bool mWritable = false;
        uint32_t mSizeInBytes = 0;
        VkShaderStageFlags mShaderStages = 0;
        VkFormat mFormat = VK_FORMAT_UNDEFINED;
        uint32_t mArrayLength = 1;
        std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> mBlockFields;
    };
}
#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    using SET_ID = uint32_t;
    using BINDING_ID = uint32_t;
    using PACKED_BINDING_ID = uint64_t;

    class GIShaderVariableVk
    {
    public:
        GIShaderVariableVk(uint32_t id); // Could be location/constId/inputAttachmentId
        GIShaderVariableVk(SET_ID set, BINDING_ID binding);

        std::optional<uint32_t> GetLocation() const;
        std::optional<uint32_t> GetConstId() const;
        std::optional<uint32_t> GetInputAttachmentId() const;
        std::optional<std::pair<SET_ID, BINDING_ID>> GetBinding() const;
        std::optional<PACKED_BINDING_ID> GetBindingPacked() const; //  high word for set-id, low word for binding-id

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
        std::optional<std::pair<SET_ID, BINDING_ID>> mBinding; // For descriptor binding variable
        
        std::string mName;
        bool mWritable = false;
        uint32_t mSizeInBytes = 0;
        VkShaderStageFlags mShaderStages = 0;
        VkFormat mFormat = VK_FORMAT_UNDEFINED;
        uint32_t mArrayLength = 1;
        std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> mBlockFields;
    };
}
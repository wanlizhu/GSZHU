#include "spirv_cross/spirv_glsl.hpp"
#include "GIShaderReflectionVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    VkShaderStageFlagBits GetShaderStageFlag(const std::wstring& path)
    {
        static std::unordered_map<std::wstring, VkShaderStageFlagBits> stageFlagsTable =
        {
            { L".vert", VK_SHADER_STAGE_VERTEX_BIT },
            { L".vert.glsl", VK_SHADER_STAGE_VERTEX_BIT },
            { L".tesc", VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT },
            { L".tesc.glsl", VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT },
            { L".tese", VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT },
            { L".tese.glsl", VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT },
            { L".geom", VK_SHADER_STAGE_GEOMETRY_BIT },
            { L".geom.glsl", VK_SHADER_STAGE_GEOMETRY_BIT },
            { L".frag", VK_SHADER_STAGE_FRAGMENT_BIT },
            { L".frag.glsl", VK_SHADER_STAGE_FRAGMENT_BIT },
            { L".comp", VK_SHADER_STAGE_COMPUTE_BIT },
            { L".comp.glsl", VK_SHADER_STAGE_COMPUTE_BIT },
        };

        for (const auto& [ext, flag] : stageFlagsTable)
        {
            if (path.find(ext) != std::wstring::npos)
            {
                return flag;
            }
        }

        return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }

    GIShaderReflectionVk::~GIShaderReflectionVk()
    {}

    void GIShaderReflectionVk::MarkPushDescriptorSet(SET_ID setId)
    {
        mIsPushDescriptorSets[setId] = true;
    }

    void GIShaderReflectionVk::MarkDynamicDescriptor(SET_ID setId, BINDING_ID binding)
    {
        mIsDynamicDescriptors[{ setId, binding}] = true;
    }

    void GIShaderReflectionVk::LoadFromJSON(const std::filesystem::path& path)
    {}

    bool GIShaderReflectionVk::IsPushDescriptorSet(SET_ID setId) const
    {
        auto it = mIsPushDescriptorSets.find(setId);
        if (it == mIsPushDescriptorSets.end())
            return false;
        else
            return it->second;
    }

    bool GIShaderReflectionVk::IsDynamicDescriptor(SET_ID setId, BINDING_ID binding) const
    {
        auto it = mIsDynamicDescriptors.find({ setId, binding });
        if (it == mIsDynamicDescriptors.end())
            return false;
        else
            return it->second;
    }

    std::optional<GIShaderVariableVk> GIShaderReflectionVk::GetVariable(const std::string& name) const
    {
        auto it = mVariables.find(name);
        if (it == mVariables.end())
            return std::nullopt;
        else
            return it->second;
    }

    std::optional<std::vector<VkDescriptorSetLayoutBinding>> GIShaderReflectionVk::GetDescriptorSetLayoutInfo(SET_ID setId) const
    {
        auto it = mDescriptorSetLayouts.find(setId);
        if (it == mDescriptorSetLayouts.end())
            return std::nullopt;
        else
            return it->second;
    }

    std::vector<VkVertexInputAttributeDescription> const& GIShaderReflectionVk::GetVertexInputAttributes() const
    {
        return mVertexInputAttributes;
    }

    std::vector<VkDescriptorPoolSize> const& GIShaderReflectionVk::GetDescriptorPoolSizes() const
    {
        return mDescriptorPoolSizes;
    }

    std::vector<VkPushConstantRange> const& GIShaderReflectionVk::GetPushConstantRanges() const
    {
        return mPushConstantRanges;
    }

    std::vector<VkSpecializationMapEntry> const& GIShaderReflectionVk::GetSpecializationMapEntries() const
    {
        return mSpecializationMapEntries;
    }

    std::unordered_map<SET_ID, std::vector<VkDescriptorSetLayoutBinding>> const& GIShaderReflectionVk::GetDescriptorSetLayoutInfos() const
    {
        return mDescriptorSetLayouts;
    }

    std::unordered_map<std::string, GIShaderVariableVk> const& GIShaderReflectionVk::GetVariables() const
    {
        return mVariables;
    }
}
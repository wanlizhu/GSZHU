#include "spirv_cross/spirv_glsl.hpp"
#include "SPIRVReflection.h"
#include "GIDeviceVk.h"
#include <fstream>
#include <sstream>

namespace AutoCAD::Graphics::Engine
{
    size_t SPIRVBindingPoint::operator()(const SPIRVBindingPoint& rhs) const
    {
        return std::hash<uint32_t>()(rhs.set.has_value() ? rhs.set.value() : 0) ^ 
            std::hash<uint32_t>()(rhs.binding);
    }

    bool SPIRVBindingPoint::operator==(const SPIRVBindingPoint& rhs) const
    {
        int setL = set.has_value() ? (int)set.value() : -1;
        int setR = rhs.set.has_value() ? (int)rhs.set.value() : -1;
        return setL == setR && binding == rhs.binding;
    }

    bool SPIRVBindingPoint::operator!=(const SPIRVBindingPoint& rhs) const
    {
        return !(*this == rhs);
    }

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

    SharedPtr<SPIRVReflection> SPIRVReflection::Create()
    {
        return SharedPtr<SPIRVReflection>(new SPIRVReflection());
    }

    SPIRVReflection::~SPIRVReflection()
    {}

    void SPIRVReflection::AddShaderStage(const std::filesystem::path& stage)
    {
#ifdef ENABLE_SPIRV_CROSS_JSON
        LoadJSON(std::filesystem::path(stage.wstring() + L".json"));
#else
        CompileToGLSL(stage);
#endif
    }

    void SPIRVReflection::AddShaderStages(const std::vector<std::filesystem::path>& stages)
    {
        for (const auto& stage : stages)
        {
            AddShaderStage(stage);
        }
    }

    void SPIRVReflection::MarkPushDescriptorSet(uint32_t setId)
    {
        mIsPushDescriptorSets[setId] = true;
    }

    void SPIRVReflection::MarkDynamicDescriptor(uint32_t setId, uint32_t binding)
    {
        mIsDynamicDescriptors[{ setId, binding }] = true;
    }

    bool SPIRVReflection::IsPushDescriptorSet(uint32_t setId) const
    {
        auto it = mIsPushDescriptorSets.find(setId);
        if (it == mIsPushDescriptorSets.end())
            return false;
        else
            return it->second;
    }

    bool SPIRVReflection::IsDynamicDescriptor(uint32_t setId, uint32_t binding) const
    {
        auto it = mIsDynamicDescriptors.find({ setId, binding });
        if (it == mIsDynamicDescriptors.end())
            return false;
        else
            return it->second;
    }

    std::optional<SPIRVVariable> SPIRVReflection::FindByName(const std::string& name) const
    {
        auto it = mVariables.find(name);
        if (it == mVariables.end())
            return std::nullopt;
        else
            return it->second;
    }

    std::vector<VkVertexInputAttributeDescription> const& SPIRVReflection::GetVertexInputAttributes() const
    {
        return mVertexInputAttributes;
    }

    std::vector<VkDescriptorPoolSize> const& SPIRVReflection::GetDescriptorPoolRequirements() const
    {
        return mDescriptorPoolRequirements;
    }

    std::vector<VkPushConstantRange> const& SPIRVReflection::GetPushConstantRanges() const
    {
        return mPushConstantRanges;
    }

    std::vector<VkSpecializationMapEntry> const& SPIRVReflection::GetSpecializationMapEntries() const
    {
        return mSpecializationMapEntries;
    }

    std::vector<uint32_t> SPIRVReflection::GetDescriptorSetIds() const
    {
        std::vector<uint32_t> ids;
        for (const auto& [setId, setBindings] : mDescriptorSetLayouts)
        {
            ids.push_back(setId);
        }
        return ids;
    }

    std::vector<VkDescriptorSetLayoutBinding> SPIRVReflection::GetDescriptorSetLayoutBindings(uint32_t setId) const
    {
        auto it = mDescriptorSetLayouts.find(setId);
        if (it == mDescriptorSetLayouts.end())
            return {};
        else
            return it->second;
    }

    void SPIRVReflection::CompileToGLSL(const std::filesystem::path& spirvPath)
    {
        // TODO: shader reflection
    }

    void SPIRVReflection::LoadJSON(const std::filesystem::path& jsonPath)
    {
        // TODO: shader reflection
    }
}
#include "spirv_cross/spirv_glsl.hpp"
#include "SPIRVReflection.h"
#include "GIDeviceVk.h"
#include <fstream>
#include <sstream>

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

    SharedPtr<SPIRVReflection> SPIRVReflection::Create()
    {
        return SharedPtr<SPIRVReflection>(new SPIRVReflection());
    }

    SPIRVReflection::~SPIRVReflection()
    {}

    void SPIRVReflection::AddShaderStage(const std::filesystem::path& stage)
    {
        // TODO: shader reflection
    }

    void SPIRVReflection::AddShaderStages(const std::vector<std::filesystem::path>& stages)
    {
        for (const auto& stage : stages)
        {
            AddShaderStage(stage);
        }
    }

    std::optional<SPIRVBlock> SPIRVReflection::GetBlock(const std::string& typeName) const
    {
        const auto& it = mBlocks.find(typeName);
        if (it == mBlocks.end())
            return std::nullopt;
        else
            return it->second;
    }

    std::optional<SPIRVArray> SPIRVReflection::GetArray(const std::string& typeName) const
    {
        const auto& it = mArrays.find(typeName);
        if (it == mArrays.end())
            return std::nullopt;
        else
            return it->second;
    }

    std::optional<SPIRVResource> SPIRVReflection::GetVariable(const std::string& name) const
    {
        const auto& it = mVariables.find(name);
        if (it == mVariables.end())
            return std::nullopt;
        else
            return it->second;
    }

    std::vector<uint32_t> SPIRVReflection::GetDescriptorSetLayoutIndices() const
    {
        std::vector<uint32_t> indices;
        for (const auto& [index, layoutBindings] : mDescriptorSetLayouts)
        {
            indices.push_back(index);
        }
        return indices;
    }

    std::vector<VkDescriptorSetLayoutBinding> const& SPIRVReflection::GetDescriptorSetLayoutBindings(uint32_t setIndex) const
    {
        const auto& it = mDescriptorSetLayouts.find(setIndex);
        if (it == mDescriptorSetLayouts.end())
            return {};
        else
            return it->second;
    }

    std::vector<VkDescriptorPoolSize> const& SPIRVReflection::GetDescriptorPoolSizes() const
    {
        return mDescriptorPoolSizes;
    }

    std::vector<VkPushConstantRange> const& SPIRVReflection::GetPushConstantRanges() const
    {
        return mPushConstantRanges;
    }
}
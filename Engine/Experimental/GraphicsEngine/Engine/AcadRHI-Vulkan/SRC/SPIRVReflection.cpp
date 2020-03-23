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

    void SPIRVReflection::AddPushDescriptorSet(uint32_t setId)
    {
        mIsPushDescriptorSets[setId] = true;
    }

    std::unordered_map<std::string, SPIRVAttribute> const& SPIRVReflection::GetAttributes() const
    {
        return mAttributes;
    }

    std::unordered_map<std::string, SPIRVBlock> const& SPIRVReflection::GetBlocks() const
    {
        return mBlocks;
    }

    std::unordered_map<std::string, SPIRVArray> const& SPIRVReflection::GetArrays() const
    {
        return mArrays;
    }

    std::vector<VkDescriptorPoolSize> const& SPIRVReflection::GetDescriptorPoolSizes() const
    {
        return mDescriptorPoolSizes;
    }

    std::vector<VkPushConstantRange> const& SPIRVReflection::GetPushConstantRanges() const
    {
        return mPushConstantRanges;
    }

    bool SPIRVReflection::IsPushDescriptorSet(uint32_t setId) const
    {
        auto it = mIsPushDescriptorSets.find(setId);
        if (it == mIsPushDescriptorSets.end())
            return false;
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
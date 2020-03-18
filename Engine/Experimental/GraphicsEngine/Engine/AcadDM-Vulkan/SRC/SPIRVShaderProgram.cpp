#include "SPIRVShaderProgram.h"
#include "GIDeviceVk.h"
#include <fstream>
#include <sstream>

namespace AutoCAD::Graphics::Engine
{
    VkShaderStageFlagBits GetShaderStageFlag(const fs::path& path)
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

        const std::wstring& strpath = path.wstring();
        for (const auto& [ext, flag] : stageFlagsTable)
        {
            if (strpath.find(ext) != std::wstring::npos)
            {
                return flag;
            }
        }

        return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }

    SharedPtr<SPIRVShaderStage> SPIRVShaderStage::Create(SharedPtr<GIDeviceVk> device, const std::wstring& filename)
    {
        std::ifstream ifs(filename.c_str(), std::ios::binary);
        if (ifs.is_open())
        {
            ifs.seekg(0, std::ios::end);
            size_t size = ifs.tellg();
            ifs.seekg(0, std::ios::beg);

            std::vector<char> buffer(size, 0); // This will be std::move() into SPIRVShaderStage
            ifs.read(buffer.data(), size);
            ifs.close();

            auto shaderStage = SharedPtr<SPIRVShaderStage>(new SPIRVShaderStage(
                device,
                std::move(buffer)));
            shaderStage->SetStageFlag(GetShaderStageFlag(fs::path(filename)));

            assert(shaderStage->IsValid());
            return shaderStage;
        }

        return nullptr;
    }

    SPIRVShaderStage::SPIRVShaderStage(SharedPtr<GIDeviceVk> device, std::vector<char>&& source)
        : GIDeviceObjectVk(device)
        , mSourceCode(std::move(source))
    {
        /*
         * If pCode points to SPIR-V code, codeSize must be a multiple of 4.
         * pCode must point to either valid SPIR-V code, formatted and packed as described by the Khronos SPIR-V Specification
         * or valid GLSL code which must be written to the GL_KHR_vulkan_glsl (specified in shader source) extension specification
        */
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.pCode = (const uint32_t*)mSourceCode.data();
        createInfo.codeSize = mSourceCode.size();

        VK_CHECK(vkCreateShaderModule(*mDevice, &createInfo, nullptr, &mShaderModule));
    }

    SPIRVShaderStage::~SPIRVShaderStage()
    {
        if (IsValid())
        {
            vkDestroyShaderModule(*mDevice, mShaderModule, nullptr);
            mShaderModule = VK_NULL_HANDLE;
        }
    }

    bool SPIRVShaderStage::IsValid() const
    {
        return mShaderModule != VK_NULL_HANDLE;
    }

    void SPIRVShaderStage::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mShaderModule,
            VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT,
            name);
    }

    void SPIRVShaderStage::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mShaderModule,
            VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT,
            tag);
    }

    void SPIRVShaderStage::SetEntryPoint(const std::string& name)
    {
        mEntryPoint = name;
    }

    void SPIRVShaderStage::SetStageFlag(VkShaderStageFlagBits flag)
    {
        mStageFlag = flag;
    }

    void SPIRVShaderStage::SetSpecializationInfo(const VkSpecializationInfo& info)
    {
        mSpecializationInfo = info;
    }

    VkShaderStageFlagBits SPIRVShaderStage::GetStageFlag() const
    {
        return mStageFlag;
    }

    VkShaderModule SPIRVShaderStage::GetShaderModule() const
    {
        return mShaderModule;
    }

    std::string const& SPIRVShaderStage::GetEntryPoint() const
    {
        return mEntryPoint;
    }

    std::optional<VkSpecializationInfo> const& SPIRVShaderStage::GetSpecializationInfo() const
    {
        return mSpecializationInfo;
    }

    std::vector<char> const& SPIRVShaderStage::GetSourceCode() const
    {
        return mSourceCode;
    }

    SharedPtr<SPIRVShaderProgram> SPIRVShaderProgram::Create()
    {
        return SharedPtr<SPIRVShaderProgram>(new SPIRVShaderProgram());
    }

    SPIRVShaderProgram::SPIRVShaderProgram()
    {}

    SPIRVShaderProgram::~SPIRVShaderProgram()
    {}

    bool SPIRVShaderProgram::IsValid() const
    {
        return !mShaderStages.empty() && std::all_of(
            mShaderStages.begin(),
            mShaderStages.end(), 
            [&](const SharedPtr<SPIRVShaderStage>& stage) {
                return stage != nullptr;
            });
    }

    void SPIRVShaderProgram::AddShaderStage(SharedPtr<SPIRVShaderStage> stage)
    {
        mShaderStages.push_back(stage);
        CreateReflection(stage->GetSourceCode());
    }

    void SPIRVShaderProgram::AddShaderStages(const std::vector<SharedPtr<SPIRVShaderStage>>& stages)
    {
        for (const auto& stage : stages)
        {
            AddShaderStage(stage);
        }
    }

    std::vector<uint32_t> SPIRVShaderProgram::GetDescriptorSetLayoutIndices() const
    {
        std::vector<uint32_t> indices;

        for (const auto& [index, setLayoutBindings] : mDescriptorSetLayouts)
        {
            indices.push_back(index);
        }

        return indices;
    }

    std::vector<VkDescriptorSetLayoutBinding> const& SPIRVShaderProgram::GetDescriptorSetLayoutBindings(uint32_t setIndex) const
    {
        const auto& it = mDescriptorSetLayouts.find(setIndex);
        if (it == mDescriptorSetLayouts.end())
            return {};
        else
            return it->second;
    }

    std::vector<VkDescriptorPoolSize> const& SPIRVShaderProgram::GetDescriptorPoolSizes() const
    {
        return mDescriptorPoolSizes;
    }

    std::vector<VkPushConstantRange> SPIRVShaderProgram::GetPushConstantRanges() const
    {
        std::vector<VkPushConstantRange> ranges;

        for (const auto& [name, pushConstant] : mPushConstants)
        {
            ranges.push_back(*mPushConstants);
        }

        return ranges;
    }

    std::vector<VkVertexInputAttributeDescription> SPIRVShaderProgram::GetVertexAttributes() const;
    std::vector<VkPipelineShaderStageCreateInfo> SPIRVShaderProgram::GetShaderStageCreateInfos() const;

    std::optional<SPIRVShaderStage> SPIRVShaderProgram::GetShaderStage(VkShaderStageFlagBits stageFlag) const;
    std::optional<SPIRVAttribute> SPIRVShaderProgram::GetVertexAttribute(const std::wstring& name) const;
    std::optional<SPIRVUniformVariable> SPIRVShaderProgram::GetUniformVariable(const std::wstring& name) const;
    std::optional<SPIRVStorageVariable> SPIRVShaderProgram::GetStorageVariable(const std::wstring& name) const;
    std::optional<SPIRVUniformBlock> SPIRVShaderProgram::GetUniformBlock(const std::wstring& name) const;
    std::optional<SPIRVStorageBlock> SPIRVShaderProgram::GetStorageBlock(const std::wstring& name) const;
    std::optional<SPIRVPushConstant> SPIRVShaderProgram::GetPushConstant(const std::wstring& name) const;
    std::optional<SPIRVShaderStage> SPIRVShaderProgram::GetShaderStage(VkShaderStageFlagBits stage) const;
}
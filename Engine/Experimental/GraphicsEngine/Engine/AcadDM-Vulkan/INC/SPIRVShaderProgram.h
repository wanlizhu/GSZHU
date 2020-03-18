#pragma once

#include "GIDeviceObjectVk.h"
#include "SPIRVShaderVariables.h"
#include <filesystem>

namespace AutoCAD::Graphics::Engine
{
    namespace fs = std::filesystem;

    class SPIRVShaderStage : public GIDeviceObjectVk
    {
    public:
        static SharedPtr<SPIRVShaderStage> Create(SharedPtr<GIDeviceVk> device, const std::wstring& filename);
        virtual ~SPIRVShaderStage();

        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        void SetEntryPoint(const std::string& name);
        void SetStageFlag(VkShaderStageFlagBits flag);
        void SetSpecializationInfo(const VkSpecializationInfo& info);

        VkShaderStageFlagBits GetStageFlag() const;
        VkShaderModule GetShaderModule() const;
        std::string const& GetEntryPoint() const;
        std::optional<VkSpecializationInfo> const& GetSpecializationInfo() const;
        std::vector<char> const& GetSourceCode() const;

    protected:
        SPIRVShaderStage(SharedPtr<GIDeviceVk> device, std::vector<char>&& source);
        SPIRVShaderStage(const SPIRVShaderStage&) = delete;
        SPIRVShaderStage(SPIRVShaderStage&&) = default;
        SPIRVShaderStage& operator=(const SPIRVShaderStage&) = delete;
        SPIRVShaderStage& operator=(SPIRVShaderStage&&) = default;

    private:
        VkShaderStageFlagBits mStageFlag = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
        VkShaderModule mShaderModule = VK_NULL_HANDLE;
        std::string mEntryPoint = "main";
        std::optional<VkSpecializationInfo> mSpecializationInfo;
        std::vector<char> mSourceCode;
    };

    class SPIRVShaderProgram
    {
        friend class SPIRVShaderProgramBuilder;
    public:
        static SharedPtr<SPIRVShaderProgram> Create();
        virtual ~SPIRVShaderProgram();
        
        bool IsValid() const;
        void AddShaderStage(SharedPtr<SPIRVShaderStage> stage);
        void AddShaderStages(const std::vector<SharedPtr<SPIRVShaderStage>>& stages);
        
        /*
         * The set of sets that are accessible to a pipeline are grouped into another object: the pipeline layout.
         * A descriptor set is a set of resources that are bound into the pipeline as a group. 
         * Multiple sets can be bound to a pipeline at a time. 
         * Each set has a layout, which describes the order and types of resources in the set. 
         * Two sets with the same layout are considered to be compatible and interchangeable. 
         * The descriptor set layout is represented by an object, and sets are created with respect to this object. 
        */
        std::vector<uint32_t> GetDescriptorSetLayoutIndices() const;
        std::vector<VkDescriptorSetLayoutBinding> const& GetDescriptorSetLayoutBindings(uint32_t setIndex) const;
        std::vector<VkDescriptorPoolSize> const& GetDescriptorPoolSizes() const;
        std::vector<VkPushConstantRange> GetPushConstantRanges() const;
        std::vector<VkVertexInputAttributeDescription> GetVertexAttributes() const;
        std::vector<VkPipelineShaderStageCreateInfo> GetShaderStageCreateInfos() const;

        std::optional<SPIRVShaderStage> GetShaderStage(VkShaderStageFlagBits stageFlag) const;
        std::optional<SPIRVAttribute> GetVertexAttribute(const std::wstring& name) const;
        std::optional<SPIRVUniformVariable> GetUniformVariable(const std::wstring& name) const;
        std::optional<SPIRVStorageVariable> GetStorageVariable(const std::wstring& name) const;
        std::optional<SPIRVUniformBlock> GetUniformBlock(const std::wstring& name) const;
        std::optional<SPIRVStorageBlock> GetStorageBlock(const std::wstring& name) const;
        std::optional<SPIRVPushConstant> GetPushConstant(const std::wstring& name) const;
        std::optional<SPIRVShaderStage> GetShaderStage(VkShaderStageFlagBits stage) const;
        
    protected:
        SPIRVShaderProgram();
        SPIRVShaderProgram(const SPIRVShaderProgram&) = delete;
        SPIRVShaderProgram(SPIRVShaderProgram&&) = default;
        SPIRVShaderProgram& operator=(const SPIRVShaderProgram&) = delete;
        SPIRVShaderProgram& operator=(SPIRVShaderProgram&&) = default;

        void CreateReflection(const std::vector<char>& source);
        void CreateReflectionSPIRV(const std::vector<char>& source);
        void CreateReflectionGLSL(const std::vector<char>& source);

    private:
        std::vector<SharedPtr<SPIRVShaderStage>> mShaderStages;
        std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>> mDescriptorSetLayouts; // A pipeline could contain multiple descriptor sets
        std::vector<VkDescriptorPoolSize> mDescriptorPoolSizes;

        std::unordered_map<std::wstring, SPIRVAttribute> mVertexAttributes;
        std::unordered_map<std::wstring, SPIRVUniformVariable> mUniformVariables;
        std::unordered_map<std::wstring, SPIRVStorageVariable> mStorageVariables;
        std::unordered_map<std::wstring, SPIRVUniformBlock> mUniformBlocks;
        std::unordered_map<std::wstring, SPIRVStorageBlock> mStorageBlocks;
        std::unordered_map<std::wstring, SPIRVPushConstant> mPushConstants;
    };
}
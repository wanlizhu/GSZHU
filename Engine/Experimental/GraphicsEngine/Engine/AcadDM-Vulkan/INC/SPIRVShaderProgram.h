#pragma once

#include "GICommonVk.h"
#include "SPIRVShaderVariables.h"
#include <filesystem>

namespace AutoCAD::Graphics::Engine
{
    namespace fs = std::filesystem;

    class SPIRVShaderProgram
    {
        friend class SPIRVShaderProgramBuilder;
    public:
        class SPIRVShaderStage
        {
        public:
            static SharedPtr<SPIRVShaderStage> CreateFromFile(const fs::path& path);
            static SharedPtr<SPIRVShaderStage> CreateFromMemory(VkShaderStageFlagBits stage, const void* data, uint32_t size);
            virtual ~SPIRVShaderStage();

            bool IsValid() const;
            void SetEntryPoint(const std::string& entryPoint);
            void SetSpecializationInfo(const VkSpecializationInfo& info);

            VkShaderStageFlagBits GetStageFlag() const;
            VkShaderModule GetShaderModule() const;
            std::string const& GetEntryPoint() const;
            std::optional<VkSpecializationInfo> const& GetSpecializationInfo() const;

        protected:
            SPIRVShaderStage() = default;
            void CreateReflectionRT(); // Create SPIR-V shader reflection at runtime (requires linking to SPIRV-Cross)
            void LoadReflectionJSON(); // Load precompiled reflection info from JSON file (no external library dependency)

        private:
            VkShaderStageFlagBits mStageFlag;
            VkShaderModule mShaderModule = VK_NULL_HANDLE;
            std::string mEntryPoint = "main";
            std::optional<VkSpecializationInfo> mSpecializationInfo;
        };

        static SharedPtr<SPIRVShaderProgram> Create();
        virtual ~SPIRVShaderProgram();
        
        bool IsValid() const;
        void AddShaderStage(SharedPtr<SPIRVShaderStage> stage);

        std::vector<SPIRVShaderStage> const& GetShaderStages() const;
        std::vector<VkDescriptorSetLayoutBinding> const& GetDescriptorSetLayoutBindings() const;
        std::vector<VkDescriptorPoolSize> const& GetDescriptorPoolSizes() const;
        std::unordered_map<std::string, SPIRVAttribute> const& GetVertexAttributes() const;
        std::unordered_map<std::string, SPIRVUniformVariable> const& GetUniformVariables() const;
        std::unordered_map<std::string, SPIRVStorageVariable> const& GetStorageVariables() const;
        std::unordered_map<std::string, SPIRVUniformBlock> const& GetUniformBlocks() const;
        std::unordered_map<std::string, SPIRVStorageBlock> const& GetStorageBlocks() const;
        std::unordered_map<std::string, SPIRVPushConstant> const& GetPushConstants() const;

        std::optional<SPIRVShaderStage> GetShaderStage(VkShaderStageFlagBits stageFlag) const;
        std::optional<SPIRVAttribute> GetVertexAttribute(const std::string& name) const;
        std::optional<SPIRVUniformVariable> GetUniformVariable(const std::string& name) const;
        std::optional<SPIRVStorageVariable> GetStorageVariable(const std::string& name) const;
        std::optional<SPIRVUniformBlock> GetUniformBlock(const std::string& name) const;
        std::optional<SPIRVStorageBlock> GetStorageBlock(const std::string& name) const;
        std::optional<SPIRVPushConstant> GetPushConstant(const std::string& name) const;
    
    protected:
        SPIRVShaderProgram();
        SPIRVShaderProgram(const SPIRVShaderProgram&) = delete;
        SPIRVShaderProgram(SPIRVShaderProgram&&) = default;
        SPIRVShaderProgram& operator=(const SPIRVShaderProgram&) = delete;
        SPIRVShaderProgram& operator=(SPIRVShaderProgram&&) = default;

    private:
        std::vector<SharedPtr<SPIRVShaderStage>> mShaderStages;
        std::vector<VkDescriptorSetLayoutBinding> mDescriptorSetLayoutBindings;
        std::vector<VkDescriptorPoolSize> mDescriptorPoolSizes;

        std::unordered_map<std::string, SPIRVAttribute> mVertexAttributes;
        std::unordered_map<std::string, SPIRVUniformVariable> mUniformVariables;
        std::unordered_map<std::string, SPIRVStorageVariable> mStorageVariables;
        std::unordered_map<std::string, SPIRVUniformBlock> mUniformBlocks;
        std::unordered_map<std::string, SPIRVStorageBlock> mStorageBlocks;
        std::unordered_map<std::string, SPIRVPushConstant> mPushConstants;
    };
}
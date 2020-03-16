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
        static void AddSearchPath(const fs::path& path);
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

    protected:
        SPIRVShaderStage(SharedPtr<GIDeviceVk> device, std::vector<char>&& source);
        SPIRVShaderStage(const SPIRVShaderStage&) = delete;
        SPIRVShaderStage(SPIRVShaderStage&&) = default;
        SPIRVShaderStage& operator=(const SPIRVShaderStage&) = delete;
        SPIRVShaderStage& operator=(SPIRVShaderStage&&) = default;

    private:
        static std::vector<fs::path> smSearchPaths;

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

        void Reflect(const std::vector<char>& source);
        void ReflectSPIRV(const std::vector<char>& source);
        void ReflectGLSL(const std::vector<char>& source);

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
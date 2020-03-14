#pragma once

#include "GICommonVk.h"
#include "SPIRVShaderVariables.h"
#include <filesystem>

// To enable SPIRV reflection, link to SPIRV-Cross official library
#define ENABLE_SPIRV_REFLECTION  1
// To enable runtime compiling (GLSL/HLSL to SPIRV), link to glslang official library
#define ENABLE_RUNTIME_COMPILE   0

namespace AutoCAD::Graphics::Engine
{
    class SPIRVShaderProgram
    {
        friend class SPIRVShaderProgramBuilder;
    public:
        class SPIRVShaderStage
        {
        public:
            static SharedPtr<SPIRVShaderStage> Create();
            static SharedPtr<SPIRVShaderStage> Create(const std::filesystem::path& path); // When the filename has extension: '.vert/.tesc/.tese/.geom/.frag/...'
            static SharedPtr<SPIRVShaderStage> Create(VkShaderStageFlagBits stageFlag, const std::filesystem::path& path); // When the filename has no valid extenson
            static SharedPtr<SPIRVShaderStage> CreateFromGLSL(const std::filesystem::path& path);
            static SharedPtr<SPIRVShaderStage> CreateFromGLSL(VkShaderStageFlagBits stageFlag, const std::filesystem::path& path);
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

        private:
            VkShaderStageFlagBits mStageFlag;
            VkShaderModule mShaderModule = VK_NULL_HANDLE;
            std::string mEntryPoint = "main";
            std::optional<VkSpecializationInfo> mSpecializationInfo;
        };

        class VertexAttribute
        {
        public:
            VertexAttribute(uint32_t loc, uint32_t offset, VkFormat format);
            VertexAttribute(uint32_t loc, VkFormat format);

            uint32_t GetLocation() const;
            uint32_t GetOffset() const;
            VkFormat GetFormat() const;

        private:
            uint32_t mLocation = 0;
            uint32_t mOffset = 0;
            VkFormat mFormat = VK_FORMAT_UNDEFINED;
        };

        static SharedPtr<SPIRVShaderProgram> Create();
        virtual ~SPIRVShaderProgram();
        virtual void CreateReflection(); // Override this when SPIRV_REFLECTION is disabled

        bool IsValid() const;
        void AddShaderStage(SharedPtr<SPIRVShaderStage> stage);
        
        std::vector<SPIRVShaderStage> const& GetShaderStages() const;
        std::vector<VertexAttribute> const& GetVertexAttributes() const;
        std::vector<VkDescriptorSetLayoutBinding> const& GetDescriptorSetLayoutBindings() const;
        std::vector<VkDescriptorPoolSize> const& GetDescriptorPoolSizes() const;
        std::unordered_map<std::string, SPIRVUniformVariable> const& GetUniformVariables() const;
        std::unordered_map<std::string, SPIRVStorageVariable> const& GetStorageVariables() const;
        std::unordered_map<std::string, SPIRVUniformBlock> const& GetUniformBlocks() const;
        std::unordered_map<std::string, SPIRVStorageBlock> const& GetStorageBlocks() const;
        std::unordered_map<std::string, SPIRVPushConstant> const& GetPushConstants() const;

        std::optional<SPIRVShaderStage> GetShaderStage(VkShaderStageFlagBits stageFlag) const;
        std::optional<VertexAttribute> GetVertexAttribute(const std::string& name) const;
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
        std::vector<VertexAttribute> mVertexAttributes;
        std::vector<VkDescriptorSetLayoutBinding> mDescriptorSetLayoutBindings;
        std::vector<VkDescriptorPoolSize> mDescriptorPoolSizes;

        std::unordered_map<std::string, SPIRVUniformVariable> mUniformVariables;
        std::unordered_map<std::string, SPIRVStorageVariable> mStorageVariables;
        std::unordered_map<std::string, SPIRVUniformBlock> mUniformBlocks;
        std::unordered_map<std::string, SPIRVStorageBlock> mStorageBlocks;
        std::unordered_map<std::string, SPIRVPushConstant> mPushConstants;
    };
}
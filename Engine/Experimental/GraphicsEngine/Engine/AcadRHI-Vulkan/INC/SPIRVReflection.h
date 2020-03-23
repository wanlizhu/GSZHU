#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    struct SPIRVAttribute
    {
        std::string name;
        uint32_t locationId = 0;
        uint32_t size = 0;
        VkFormat format = VK_FORMAT_UNDEFINED; // VK_FORMAT_UNDEFINED for block and array type
        std::optional<uint32_t> arrayLength;
    };

    struct SPIRVVariable
    {
        std::string name;
        uint32_t setId = 0;
        uint32_t bindingId = 0;
        uint32_t size = 0;
        VkShaderStageFlags stageFlags = 0;
        bool isWritable = false;
    };

    struct SPIRVBlock : public SPIRVVariable
    {
        struct BlockField
        {
            std::string name;
            uint32_t offset = 0;
            uint32_t size = 0;
            VkFormat format = VK_FORMAT_UNDEFINED; // VK_FORMAT_UNDEFINED for array type here (nested block is not allowed)
            std::optional<uint32_t> arrayLength;
        };

        std::unordered_map<std::string, BlockField> members;
    };

    struct SPIRVArray : public SPIRVVariable
    {
        uint32_t arrayLength = 0;
    };

    struct SPIRVConstantVariable
    {
        std::string name;
        uint32_t size = 0;
        VkShaderStageFlags stageFlag = 0;
    };

    struct SPIRVPushConstant : public SPIRVConstantVariable
    {
        struct BlockField
        {
            std::string name;
            uint32_t offset = 0;
            uint32_t size = 0;
            VkFormat format = VK_FORMAT_UNDEFINED; // VK_FORMAT_UNDEFINED for array type here (nested block is not allowed)
            std::optional<uint32_t> arrayLength;
        };

        std::unordered_map<std::string, BlockField> members;
    };

    struct SPIRVSpecializationConstant : public SPIRVConstantVariable
    {
        uint32_t constId = 0;
    };

    struct SPIRVOpaqueVariable
    {
        std::string name;
    };

    struct SPIRVSampledImage : public SPIRVOpaqueVariable
    {
        std::string name;
    };

    struct SPIRVImage : public SPIRVOpaqueVariable
    {
        std::string name;
        bool isWritable = false;
    };

    struct SPIRVSampler : public SPIRVOpaqueVariable
    {
        std::string name;
    };


    /* 'non-opaque uniforms outside a block' : not allowed when using GLSL for Vulkan.
     * 'sampler2D' : sampler/image types can only be used in uniform variables or function parameters.
     * 'buffer' : buffers can be declared only as blocks.
    */
    class SPIRVReflection
    {
    public:
        static SharedPtr<SPIRVReflection> Create();
        virtual ~SPIRVReflection();

        void AddShaderStage(const std::filesystem::path& stage);
        void AddShaderStages(const std::vector<std::filesystem::path>& stages);
        void AddPushDescriptorSet(uint32_t setId); // This information can only be specified from application, instead of shader

        std::unordered_map<std::string, SPIRVAttribute> const& GetAttributes() const;
        std::unordered_map<std::string, SPIRVBlock> const& GetBlocks() const;
        std::unordered_map<std::string, SPIRVArray> const& GetArrays() const;
        
        /*
         * The set of sets that are accessible to a pipeline are grouped into another object: the pipeline layout.
         * A descriptor set is a set of resources that are bound into the pipeline as a group. 
         * Multiple sets can be bound to a pipeline at a time. 
         * Each set has a layout, which describes the order and types of resources in the set. 
         * Two sets with the same layout are considered to be compatible and interchangeable. 
         * The descriptor set layout is represented by an object, and sets are created with respect to this object. 
        */
        std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>> const& GetDescriptorSetLayoutsBindings() const;
        std::vector<VkDescriptorPoolSize> const& GetDescriptorPoolSizes() const;
        std::vector<VkPushConstantRange> const& GetPushConstantRanges() const;
        bool IsPushDescriptorSet(uint32_t setId) const;

    protected:
        SPIRVReflection() = default;
        SPIRVReflection(const SPIRVReflection&) = delete;
        SPIRVReflection(SPIRVReflection&&) = default;
        SPIRVReflection& operator=(const SPIRVReflection&) = delete;
        SPIRVReflection& operator=(SPIRVReflection&&) = default;

        void CompileToGLSL(const std::filesystem::path& spirvPath); // Convert SPIRV binary code to GLSL for reflection
        void LoadJSON(const std::filesystem::path& jsonPath); // Load precompiled reflection info from file

    private:
        // A pipeline could contain multiple descriptor sets
        std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>> mDescriptorSetLayouts;
        std::unordered_map<uint32_t, bool> mIsPushDescriptorSets;
        std::vector<VkDescriptorPoolSize> mDescriptorPoolSizes;
        std::vector<VkPushConstantRange> mPushConstantRanges; // Only one push_constant block is allowed per stage.

        //std::unordered_map<std::string, SPIRVAttribute> mAttributes;
        //std::unordered_map<std::string, SPIRVUniformVariable> mUniformVariables;
        //std::unordered_map<std::string, SPIRVStorageVariable> mStorageVariables;
        //std::unordered_map<std::string, SPIRVPushConstant> mPushConstants;
        //std::unordered_map<std::string, SPIRVSpecializationConstant> mSpecializationConstants;
        //std::unordered_map<std::string, SPIRVOpaqueVariable> mOpaqueVariables;
    };


}
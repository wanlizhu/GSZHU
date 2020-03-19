#pragma once

#include "GICommonVk.h"
#include "spirv_cross/spirv_glsl.hpp"

namespace AutoCAD::Graphics::Engine
{
    enum class SPIRVType
    {
        Scalar,
        Block,
        Array,
    };

    struct SPIRVResource
    {
        std::string name;
        uint32_t size = 0;
        SPIRVType type = SPIRVType::Scalar;
        VkFormat format = VK_FORMAT_UNDEFINED;
        VkShaderStageFlags stages = 0;
        bool writable = false;

        std::optional<uint32_t> locationId;       // For shader stage input and output variable
        std::optional<uint32_t> constantId;       // For specialization constant
        std::optional<uint32_t> descriptorSetId;  // For buffer and image binding block
        std::optional<uint32_t> bindingId;        // For buffer and image binding block
        std::optional<uint32_t> offset;           // For block member
        std::optional<std::string> typeName;      // For Scalar, this is std::nullopt
    };

    struct SPIRVBlock : public SPIRVResource
    {
        std::unordered_map<std::string, SPIRVResource> members;
    };

    struct SPIRVArray : public SPIRVResource
    {
        SPIRVType elementType = SPIRVType::Scalar;
        std::optional<VkFormat> elementTypeFormat;
        std::optional<std::string> elementTypeName;

        uint32_t stride = 0;
        uint32_t length = 0;
        bool isLengthLiteral = true;
    };

    /* 'non-opaque uniforms outside a block' : not allowed when using GLSL for Vulkan
    */
    class SPIRVReflection
    {
    public:
        static SharedPtr<SPIRVReflection> Create();
        virtual ~SPIRVReflection();

        void AddShaderStage(const std::wstring& stage);
        void AddShaderStages(const std::vector<std::wstring>& stages);

        const std::optional<SPIRVResource>& GetVariable(const std::string& name) const;
        const std::optional<SPIRVBlock>& GetBlock(const std::string& typeName) const;
        const std::optional<SPIRVArray>& GetArray(const std::string& typeName) const;
        
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
        std::vector<VkPushConstantRange> const& GetPushConstantRanges() const;

    protected:
        SPIRVReflection() = default;
        SPIRVReflection(const SPIRVReflection&) = delete;
        SPIRVReflection(SPIRVReflection&&) = default;
        SPIRVReflection& operator=(const SPIRVReflection&) = delete;
        SPIRVReflection& operator=(SPIRVReflection&&) = default;

    private:
        // A pipeline could contain multiple descriptor sets
        std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>> mDescriptorSetLayouts; 
        std::vector<VkDescriptorPoolSize> mDescriptorPoolSizes;
        std::vector<VkPushConstantRange> mPushConstantRanges;

        std::unordered_map<std::string, SPIRVResource> mVariables;
        std::unordered_map<std::string, SPIRVBlock> mBlocks;
        std::unordered_map<std::string, SPIRVArray> mArrays;
    };
}
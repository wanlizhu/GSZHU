#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    struct SPIRVBindingPoint
    {
        std::optional<uint32_t> set;
        uint32_t binding = 0;

        size_t operator()(const SPIRVBindingPoint& rhs) const;
        bool operator==(const SPIRVBindingPoint& rhs) const;
        bool operator!=(const SPIRVBindingPoint& rhs) const;
    };

    struct SPIRVVariable
    {
        std::optional<uint32_t> location; // For stage input/output variable
        std::optional<uint32_t> constId;  // For specialization variable
        std::optional<uint32_t> inputAttachmentId; // For input attachment used in fragment stage
        std::optional<SPIRVBindingPoint> binding; // For descriptor binding variable
     
        std::string name;
        uint32_t size = 0;
        std::optional<uint32_t> arrayLength;
        std::optional<std::unordered_map<std::string, std::pair<uint32_t, uint32_t>>> blockFields;

        bool writable = false;
        VkShaderStageFlags shaderFlags = 0;
        std::optional<VkFormat> format;
    };

    /* 'non-opaque uniforms outside a block' : not allowed when using GLSL for Vulkan.
     * 'sampler2D' : sampler/image types can only be used in uniform variables or function parameters.
     * 'buffer' : buffers can be declared only as blocks.
     *
     * The set of sets that are accessible to a pipeline are grouped into another object: the pipeline layout.
     * A descriptor set is a set of resources that are bound into the pipeline as a group.
     * Multiple sets can be bound to a pipeline at a time.
     * Each set has a layout, which describes the order and types of resources in the set.
     * Two sets with the same layout are considered to be compatible and interchangeable.
     * The descriptor set layout is represented by an object, and sets are created with respect to this object.
    */
    class SPIRVReflection : public GINonCopyable
    {
    public:
        static SharedPtr<SPIRVReflection> Create();
        virtual ~SPIRVReflection();

        void AddShaderStage(const std::filesystem::path& stage);
        void AddShaderStages(const std::vector<std::filesystem::path>& stages);

        bool IsPushDescriptorSet(uint32_t setId) const;
        bool IsDynamicDescriptor(uint32_t setId, uint32_t binding) const;
        std::optional<SPIRVVariable> FindByName(const std::string& name) const;
        std::vector<VkVertexInputAttributeDescription> const& GetVertexInputAttributes() const;
        std::vector<VkDescriptorPoolSize> const& GetDescriptorPoolRequirements() const;
        std::vector<VkPushConstantRange> const& GetPushConstantRanges() const;
        std::vector<VkSpecializationMapEntry> const& GetSpecializationMapEntries() const;
        std::vector<VkDescriptorSetLayoutBinding> GetDescriptorSetLayoutBindings(uint32_t setId) const;
        std::vector<uint32_t> GetDescriptorSetIds() const;

        /* This information can only be specified from application, instead of shader reflection.
         * Descriptor set marked as 'push descriptor set' can only be updated using vkCmdPushDescriptorSetKHR which is for frequent updatings.
         * For uniform/storage buffers, it is better to use 'dynamic offset' for frequent updatings.
        */
        void MarkPushDescriptorSet(uint32_t setId);

        /* This information can only be specified from application, instead of shader reflection.
         * Supports two kinds of dynamic descriptors: VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC and VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC.
         * Argument 'setId' cannot be marked as 'push descriptor set'.
        */
        void MarkDynamicDescriptor(uint32_t setId, uint32_t binding);
        
    protected:
        void CompileToGLSL(const std::filesystem::path& spirvPath); // Convert SPIRV binary code to GLSL for reflection
        void LoadJSON(const std::filesystem::path& jsonPath); // Load precompiled reflection info from file

    private:
        std::vector<VkVertexInputAttributeDescription> mVertexInputAttributes; // The field 'binding' is left empty for application to assign later
        std::vector<VkDescriptorPoolSize> mDescriptorPoolRequirements; // Used to expand the size requirements of global descriptor pool
        std::vector<VkPushConstantRange> mPushConstantRanges; // Only one push_constant block is allowed per stage.
        std::vector<VkSpecializationMapEntry> mSpecializationMapEntries;
        
        std::unordered_map<uint32_t, bool> mIsPushDescriptorSets; // Assigned by application (NOT the shader reflection)
        std::unordered_map<SPIRVBindingPoint, bool, SPIRVBindingPoint> mIsDynamicDescriptors; // Assigned by application (NOT the shader reflection)
        std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>> mDescriptorSetLayouts; // A pipeline contains multiple descriptor sets
        std::unordered_map<std::string, SPIRVVariable> mVariables;
    };


}
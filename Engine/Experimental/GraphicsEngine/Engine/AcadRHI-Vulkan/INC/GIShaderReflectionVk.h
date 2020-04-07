#pragma once

#include "GIShaderVariableVk.h"

namespace AutoCAD::Graphics::Engine
{
    /* 
     * 'non-opaque uniforms outside a block' : not allowed when using GLSL for Vulkan.
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
    class GIShaderReflectionVk : public GINonCopyable
    {
        friend class GIShaderReflectionBuilderVk;
    public:
        GIShaderReflectionVk() = default;
        virtual ~GIShaderReflectionVk();
        bool IsPushDescriptorSet(SET_ID setId) const;
        bool IsDynamicDescriptor(SET_ID setId, BINDING_ID bindingId) const;
        std::optional<GIShaderVariableVk> GetVariable(const std::string& name) const;
        std::optional<std::vector<VkDescriptorSetLayoutBinding>> GetDescriptorSetLayoutInfo(SET_ID setId) const;
        
        std::vector<VkVertexInputAttributeDescription> const& GetVertexInputAttributes() const;
        std::vector<VkDescriptorPoolSize> const& GetDescriptorPoolSizes() const;
        std::vector<VkPushConstantRange> const& GetPushConstantRanges() const;
        std::vector<VkSpecializationMapEntry> const& GetSpecializationMapEntries() const;
        std::unordered_map<SET_ID, std::vector<VkDescriptorSetLayoutBinding>> const& GetDescriptorSetLayoutInfos() const;
        std::unordered_map<std::string, GIShaderVariableVk> const& GetVariables() const;

        /* 
         * This information can only be specified from application, instead of shader reflection.
         * Descriptor set marked as 'push descriptor set' can only be updated using vkCmdPushDescriptorSetKHR which is for frequent updatings.
         * For uniform/storage buffers, it is better to use 'dynamic offset' for frequent updatings.
        */
        void MarkPushDescriptorSet(SET_ID setId);

        /* 
         * This information can only be specified from application, instead of shader reflection.
         * Supports two kinds of dynamic descriptors: VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC and VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC.
         * Argument 'setId' cannot be marked as 'push descriptor set'.
        */
        void MarkDynamicDescriptor(SET_ID setId, BINDING_ID bindingId);

        /*
         * Load reflection data from json created while pre-compilation 
        */
        void LoadFromJSON(const std::filesystem::path& path);
        
    private:
        std::vector<VkVertexInputAttributeDescription> mVertexInputAttributes; // The field 'binding' is left empty for application to assign later
        std::vector<VkDescriptorPoolSize> mDescriptorPoolSizes; // Used to expand the size requirements of global descriptor pool
        std::vector<VkPushConstantRange> mPushConstantRanges; // Only one push_constant block is allowed per stage.
        std::vector<VkSpecializationMapEntry> mSpecializationMapEntries;
        
        std::unordered_map<SET_ID, bool> mIsPushDescriptorSets; // Assigned by application (NOT the shader reflection)
        std::unordered_map<GIDescriptorBindingVk, bool> mIsDynamicDescriptors; // Assigned by application (NOT the shader reflection)
        std::unordered_map<SET_ID, std::vector<VkDescriptorSetLayoutBinding>> mDescriptorSetLayouts; // A pipeline contains multiple descriptor sets
        std::unordered_map<std::string, GIShaderVariableVk> mVariables;
    };
}
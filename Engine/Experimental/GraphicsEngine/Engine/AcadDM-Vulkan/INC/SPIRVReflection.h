#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class SPIRVReflectObject : public std::enable_shared_from_this<SPIRVReflectObject>
    {
    public:
        virtual ~SPIRVReflectObject() = default;
        virtual operator bool() const = 0;
        virtual const std::string& GetName() const = 0;
        virtual VkShaderStageFlags GetStageFlags() const = 0;
        virtual uint32_t GetSize() const = 0;

        template<typename T>
        SharedPtr<T> As() 
        {
            static_assert(std::is_base_of_v<SPIRVReflectObject, T>);
            auto ptr = std::dynamic_pointer_cast<T>(shared_from_this());
            assert(ptr != nullptr);
            return ptr;
        }
    };

    class SPIRVShaderVariable : public SPIRVReflectObject
    {
    public:
        virtual bool IsPushConstant() const = 0;
        virtual bool IsUniform() const = 0;
        virtual bool IsStorage() const = 0;
        virtual bool IsSampler() const = 0;
        virtual bool IsImage() const = 0;
        virtual bool IsSampledImage() const = 0;
        virtual bool IsAtomicCounter() const = 0;
    };

    class SPIRVShaderBlock : public SPIRVReflectObject
    {

    };

    class SPIRVShaderArray : public SPIRVReflectObject
    {

    };
    
    class SPIRVReflection
    {
    public:
        static SharedPtr<SPIRVReflection> Create();
        virtual ~SPIRVReflection();

        void AddShaderStage(const std::wstring& stage);
        void AddShaderStages(const std::vector<std::wstring>& stages);

        std::optional<SharedPtr<SPIRVShaderVariable>> Get(const std::string& name) const;

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

    protected:
        SPIRVReflection();
        SPIRVReflection(const SPIRVReflection&) = delete;
        SPIRVReflection(SPIRVReflection&&) = default;
        SPIRVReflection& operator=(const SPIRVReflection&) = delete;
        SPIRVReflection& operator=(SPIRVReflection&&) = default;

    private:
        std::unordered_map<std::string, SharedPtr<SPIRVShaderVariable>> mShaderVariables;

        // A pipeline could contain multiple descriptor sets
        std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>> mDescriptorSetLayouts; 
        std::vector<VkDescriptorPoolSize> mDescriptorPoolSizes;
    };
}
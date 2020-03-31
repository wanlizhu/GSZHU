#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIVertexLayoutVk : public GINonCopyable
    {
    public:
        static SharedPtr<GIVertexLayoutVk> Create();

        bool IsValid() const;
        void AddVertexAttribute(uint32_t location, uint32_t offset, VkFormat format, uint32_t binding);
        void AddVertexBinding(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate);        
        
        std::optional<VkVertexInputBindingDescription> GetVertexBinding(uint32_t binding) const;
        std::vector<VkVertexInputBindingDescription> const& GetVertexBindings() const;
        std::vector<VkVertexInputAttributeDescription> const& GetVertexAttributes() const;

    protected:
        GIVertexLayoutVk();
        
    private:
        std::vector<VkVertexInputBindingDescription> mVertexBindings;
        std::vector<VkVertexInputAttributeDescription> mVertexAttributes;
    };
}
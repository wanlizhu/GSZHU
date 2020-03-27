#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIVertexLayoutVk : public GINonCopyable
    {
    public:
        static SharedPtr<GIVertexLayoutVk> Create();

        void AddVertexAttribute(uint32_t location, uint32_t offset, VkFormat format, uint32_t binding);
        void AddBindingPoint(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate);        
        bool IsValid() const;

        std::optional<VkVertexInputBindingDescription> GetBindingPoint(uint32_t binding) const;
        std::vector<VkVertexInputBindingDescription> const& GetBindingPoints() const;
        std::vector<VkVertexInputAttributeDescription> const& GetVertexAttributes() const;

    protected:
        GIVertexLayoutVk();
        
    private:
        std::vector<VkVertexInputBindingDescription> mBindingPoints;
        std::vector<VkVertexInputAttributeDescription> mVertexAttributes;
    };
}
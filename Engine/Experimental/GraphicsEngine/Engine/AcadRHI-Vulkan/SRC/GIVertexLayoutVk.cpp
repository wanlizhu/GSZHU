#include "GIVertexLayoutVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIVertexLayoutVk> GIVertexLayoutVk::Create()
    {
        return SharedPtr<GIVertexLayoutVk>(new GIVertexLayoutVk());
    }

    GIVertexLayoutVk::GIVertexLayoutVk()
    {}

    bool GIVertexLayoutVk::IsValid() const
    {
        return !mVertexAttributes.empty() && !mVertexBindings.empty();
    }

    void GIVertexLayoutVk::AddVertexAttribute(uint32_t location, uint32_t offset, VkFormat format, uint32_t binding)
    {
        VkVertexInputAttributeDescription attrib = {};
        attrib.location = location;
        attrib.binding = binding;
        attrib.offset = offset;
        attrib.format = format;
        mVertexAttributes.push_back(attrib);
    }

    void GIVertexLayoutVk::AddVertexBinding(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate)
    {
        VkVertexInputBindingDescription inputBinding = {};
        inputBinding.binding = binding;
        inputBinding.stride = stride;
        inputBinding.inputRate = inputRate;
        mVertexBindings.push_back(inputBinding);
    }

    std::optional<VkVertexInputBindingDescription> GIVertexLayoutVk::GetVertexBinding(uint32_t binding) const
    {
        auto it = std::find_if(
            mVertexBindings.begin(),
            mVertexBindings.end(),
            [&](const VkVertexInputBindingDescription& item) {
                return item.binding == binding;
            });

        if (it == mVertexBindings.end())
            return std::nullopt;
        else
            return *it;
    }

    std::vector<VkVertexInputBindingDescription> const& GIVertexLayoutVk::GetVertexBindings() const
    {
        return mVertexBindings;
    }

    std::vector<VkVertexInputAttributeDescription> const& GIVertexLayoutVk::GetVertexAttributes() const
    {
        return mVertexAttributes;
    }
}
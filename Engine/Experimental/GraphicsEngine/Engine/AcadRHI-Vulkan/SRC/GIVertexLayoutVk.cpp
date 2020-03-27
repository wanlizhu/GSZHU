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

    void GIVertexLayoutVk::AddVertexAttribute(uint32_t location, uint32_t offset, VkFormat format, uint32_t binding)
    {
        VkVertexInputAttributeDescription attrib = {};
        attrib.location = location;
        attrib.binding = binding;
        attrib.offset = offset;
        attrib.format = format;
        mVertexAttributes.push_back(attrib);
    }

    void GIVertexLayoutVk::AddBindingPoint(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate)
    {
        VkVertexInputBindingDescription inputBinding = {};
        inputBinding.binding = binding;
        inputBinding.stride = stride;
        inputBinding.inputRate = inputRate;
        mBindingPoints.push_back(inputBinding);
    }

    bool GIVertexLayoutVk::IsValid() const
    {
        return !mVertexAttributes.empty() && !mBindingPoints.empty();
    }

    std::optional<VkVertexInputBindingDescription> GIVertexLayoutVk::GetBindingPoint(uint32_t binding) const
    {
        auto it = std::find_if(
            mBindingPoints.begin(),
            mBindingPoints.end(),
            [&](const VkVertexInputBindingDescription& item) {
                return item.binding == binding;
            });

        if (it == mBindingPoints.end())
            return std::nullopt;
        else
            return *it;
    }

    std::vector<VkVertexInputBindingDescription> const& GIVertexLayoutVk::GetBindingPoints() const
    {
        return mBindingPoints;
    }

    std::vector<VkVertexInputAttributeDescription> const& GIVertexLayoutVk::GetVertexAttributes() const
    {
        return mVertexAttributes;
    }
}
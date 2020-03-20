#include "GIPipelineCacheVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIPipelineCacheVk> GIPipelineCacheVk::Create(SharedPtr<GIDeviceVk> device, const std::wstring& path)
    {
        return SharedPtr<GIPipelineCacheVk>(new GIPipelineCacheVk(device, path));
    }

    GIPipelineCacheVk::GIPipelineCacheVk(SharedPtr<GIDeviceVk> device, const std::wstring& path)
        : GIDeviceObjectVk(device)
        , mFilePath(path)
    {
        std::vector<char> buffer;

        if (!mFilePath.empty())
        {
            std::ifstream input(mFilePath.wstring(), std::ios::binary);
            input.seekg(0, std::ios::end);
            buffer.resize(input.tellg(), 0);
            input.seekg(0, std::ios::beg);
            input.read(buffer.data(), buffer.size());
            input.close();
        }

        VkPipelineCacheCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.initialDataSize = buffer.size();
        createInfo.pInitialData = buffer.data();

        VK_CHECK(vkCreatePipelineCache(*mDevice, &createInfo, nullptr, &mPipelineCache));
    }

    GIPipelineCacheVk::~GIPipelineCacheVk()
    {
        if (IsValid())
        {
            Flush();
            vkDestroyPipelineCache(*mDevice, mPipelineCache, nullptr);
            mPipelineCache = VK_NULL_HANDLE;
        }
    }

    GIPipelineCacheVk::operator const VkPipelineCache& () const
    {
        return mPipelineCache;
    }

    bool GIPipelineCacheVk::IsValid() const
    {
        return mPipelineCache != VK_NULL_HANDLE;
    }

    void GIPipelineCacheVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mPipelineCache,
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT,
            name);
    }

    void GIPipelineCacheVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mPipelineCache,
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT,
            tag);
    }

    void GIPipelineCacheVk::Flush() const
    {
        if (IsValid() && !mFilePath.empty())
        {
            size_t size = 0;
            VK_CHECK(vkGetPipelineCacheData(*mDevice, mPipelineCache, &size, nullptr));
            if (size > 0)
            {
                std::vector<char> data(size);
                VK_CHECK(vkGetPipelineCacheData(*mDevice, mPipelineCache, &size, data.data()));

                std::ofstream output(mFilePath.wstring(), std::ios::binary);
                if (output.is_open())
                {
                    output.write(data.data(), data.size());
                    output.close();
                }
            }
        }
    }
}
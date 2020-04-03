#include "GIInstanceBuilderVk.h"
#include "GIInstanceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIInstanceBuilderVk::GIInstanceBuilderVk()
    {
        mAppInfo = {};
        mAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        mAppInfo.pNext = nullptr;
        mAppInfo.apiVersion = VK_API_VERSION_1_2;
        mAppInfo.pApplicationName = "AutoCAD";
        mAppInfo.pEngineName = "AcadDM";

        mCreateInfo = {};
        mCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        mCreateInfo.pNext = nullptr;
        mCreateInfo.pApplicationInfo = &mAppInfo;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::SetAppName(const char* name)
    {
        mAppInfo.pApplicationName = name;
        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::SetEngineName(const char* name)
    {
        mAppInfo.pEngineName = name;
        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::SetApiVersion(uint32_t version)
    {
        mAppInfo.apiVersion = version;
        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::EnableValidationLayers(bool value)
    {
        mEnableValidationLayers = value;
        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::EnableLayers(const std::vector<const char*>& names)
    {
        for (const auto& name : names)
        {
            EnableLayer(name, true);
        }
        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::EnableLayer(const char* name, bool compulsory)
    {
        if (GIInstanceVk::IsLayerSupported(name))
        {
            mEnabledLayers.push_back(name);
        }
        else
        {
            if (compulsory)
                LOG_ERROR("Compulsory layer '%s' is not supported.\n", name);
            else
                LOG_WARNING("Optional layer '%s' is not supported.\n", name);
        }

        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::EnableExtension(const char* name, bool compulsory)
    {
        if (GIInstanceVk::IsExtensionSupported(name))
        {
            mEnabledExtensions.push_back(name);
        }
        else
        {
            if (compulsory)
                LOG_ERROR("Compulsory extension '%s' is not supported.\n", name);
            else
                LOG_WARNING("Optional extension '%s' is not supported.\n", name);
        }

        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::EnableExtensions(const std::vector<const char*>& names)
    {
        for (const auto& name : names)
        {
            EnableExtension(name, true);
        }
        return *this;
    }

    SharedPtr<GIInstanceVk> GIInstanceBuilderVk::Build()
    {
        if (mEnableValidationLayers)
        {
            mCreateInfo.enabledLayerCount = (uint32_t)mEnabledLayers.size();
            mCreateInfo.ppEnabledLayerNames = mEnabledLayers.data();
        }
        mCreateInfo.enabledExtensionCount = (uint32_t)mEnabledExtensions.size();
        mCreateInfo.ppEnabledExtensionNames = mEnabledExtensions.data();

        auto instance = SharedPtr<GIInstanceVk>(new GIInstanceVk(
            mCreateInfo,
            mEnabledLayers,
            mEnabledExtensions));
        assert(instance->IsValid());

        return instance;
    }
}
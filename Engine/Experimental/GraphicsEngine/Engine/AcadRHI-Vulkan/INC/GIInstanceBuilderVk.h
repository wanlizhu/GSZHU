#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIInstanceVk;

    class GIInstanceBuilderVk
    {
    public:
        GIInstanceBuilderVk();
        GIInstanceBuilderVk& SetAppName(const char* name);
        GIInstanceBuilderVk& SetEngineName(const char* name);
        GIInstanceBuilderVk& SetApiVersion(uint32_t version);
        GIInstanceBuilderVk& EnableValidationLayers(bool value);
        GIInstanceBuilderVk& EnableLayer(const char* name, bool compulsory = false);
        GIInstanceBuilderVk& EnableLayers(const std::vector<const char*>& names);
        GIInstanceBuilderVk& EnableExtension(const char* name, bool compulsory = false);
        GIInstanceBuilderVk& EnableExtensions(const std::vector<const char*>& names);
        SharedPtr<GIInstanceVk> Build();

    private:
        VkApplicationInfo mAppInfo = {};
        VkInstanceCreateInfo mCreateInfo = {};

        std::vector<const char*> mEnabledLayers;
        std::vector<const char*> mEnabledExtensions;
        bool mEnableValidationLayers = true;
    };
}
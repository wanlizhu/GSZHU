#pragma once

#include "VulkanRHI/VulkanConfig.h"
#include "Utilities/NonCopyable.h"
#include "VulkanDebug.h"
#include "VulkanLayer.h"
#include "VulkanPlatform.h"

namespace Wanli
{
    class DLLDECL VulkanInstance : public NonCopyable
        , public std::enable_shared_from_this<VulkanInstance>
    {
    public:
        static const PCCHArray kInstanceValidationLayers;
        static const PCCHArray kInstanceExtensions;

        VulkanInstance();
        virtual ~VulkanInstance();

        virtual bool Initialize(bool enableDebug);
        virtual void Destroy();

        inline operator const VkInstance&() const { return mVkInstance; }
        inline VkInstance GetHandle() const { return mVkInstance; }
        inline const PCCHArray& GetSupportedLayers() const { return mSupportedLayers; }
        inline const PCCHArray& GetSupportedExtensions() const { return mSupportedExtensions; }
        inline const PCCHArray& GetEnabledLayers() const { return mEnabledLayers; }
        inline const PCCHArray& GetEnabledExtensions() const { return mEnabledExtensions; }
        inline bool IsValidationLayersEnabled() const { return mEnableValidationLayers; }

        void EnableExtension(const char* extension);
        void EnableLayer(const char* layerName);
        void EnableValidationLayers(bool enable) { mEnableValidationLayers = enable; }

    protected:
        void CreateVkInstance();
        void CreateDebug(bool enableDebug);
        void LoadSupportedLayersAndExtensions();
        void AddEnabledLayersAndExtensions(bool enableDebug);
        void AddEnabledPlatformExtensions();
        void LogEnabledLayers();

    protected:
        VkInstance mVkInstance = VK_NULL_HANDLE;
        UniquePtr<VulkanDebug> mDebug;
        PCCHArray mSupportedExtensions;
        PCCHArray mSupportedLayers;
        PCCHArray mEnabledExtensions;
        PCCHArray mEnabledLayers;
        bool mEnableValidationLayers = false;
    };
}
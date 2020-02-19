#pragma once

#include "Core/BasicTypes.h"
#include "Configurations/Configuration.h"
#include "Utilities/Delegate.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    struct WindowCreateInfo
    {
        using EGraphicsAPI = ERHIDevice;

        String title = "Wanli Engine";
        glm::ivec2 pos = { -1, -1 };
        glm::ivec2 size = { 0 , 0 };

        EGraphicsAPI supportAPI = EGraphicsAPI::Vulkan;
        bool createMenu = true;
        bool fullscreen = false;
        bool resizable = true;
        bool borderless = false;
        bool floating = false;
    };

    class DLLDECL IApplication : virtual public Observer
        , public NonCopyable
        , public std::enable_shared_from_this<IApplication>
    {
    public:
        virtual ~IApplication() = default;
        virtual void Initialize(const WindowCreateInfo& info) = 0;
        virtual void Update() = 0;
        virtual void Shutdown() = 0;
        virtual bool IsQuit() const = 0;

        virtual WindowHandle GetWindowHandle() const = 0;
        virtual glm::ivec2 GetContentSize() const = 0;
        virtual glm::vec2 GetContentScale() const = 0;
        virtual PCCHArray GetRequiredVulkanExtensions() const = 0;
        virtual PCCHArray GetRequiredVulkanLayers() const = 0;
    };

    UniquePtr<IApplication> DLLDECL MakeApplication(const WindowCreateInfo& info);
}
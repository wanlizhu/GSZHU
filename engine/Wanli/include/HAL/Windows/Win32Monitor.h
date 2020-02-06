#pragma once

#include "Application/IMonitor.h"
#include "HAL/Windows/Win32Common.h"

struct GLFWmonitor;

namespace Wanli
{
    class Win32Monitor : public IMonitor
    {
    public:
        static int GetMonitors(std::vector<Win32Monitor>& monitors);

        explicit Win32Monitor(GLFWmonitor* monitor = nullptr);
        Win32Monitor(const Win32Monitor&) = default;
        Win32Monitor& operator=(const Win32Monitor&) = default;
        virtual ~Win32Monitor();

        virtual bool IsPrimary() const override final;
        virtual glm::ivec2 GetDisplaySize() const override final;
        virtual glm::ivec2 GetPhysicalSize() const override final;
        virtual glm::vec2 GetScaleFactor() const override final;
        virtual std::string GetName() const override final;
        virtual std::vector<VideoMode> GetAvailableVideoModes() const override final;
        virtual VideoMode GetVideoMode() const override final;
        virtual Gamma GetGamma() const override final;
        virtual void SetGamma(const Gamma& gamma) const override final;

    protected:
        GLFWmonitor* mGlfwHandle = nullptr;
        VideoMode mVideoMode;
        Gamma mGamma;
    };
}
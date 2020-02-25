#pragma once
#ifdef PLATFORM_WINDOWS

#include "Core/Common.h"
#include "Utils/NonCopyable.h"

struct GLFWmonitor;

namespace Wanli
{
    struct VideoMode
    {
        int width = 0;
        int height = 0;
        int redBits = 0;
        int greenBits = 0;
        int blueBits = 0;
        int refreshRate = 0;
    };

    struct GammaRamp
    {
        Uint16* red = nullptr;
        Uint16* green = nullptr;
        Uint16* blue = nullptr;
        Uint size = 0;
    };

    class Monitor : public NonCopyable
    {
    public:
        Monitor() = default;
        explicit Monitor(GLFWmonitor* monitor);
        virtual ~Monitor();

        operator bool() const;
        bool IsPrimary() const;
        GLFWmonitor* GetMonitorGLFW() const;
        glm::ivec2 GetClientAreaSize() const;
        glm::ivec2 GetClientAreaPos() const;
        glm::ivec2 GetPhysicalSize() const;
        glm::ivec2 GetPos() const;
        glm::vec2 GetContentScale() const;
        String GetName() const;
        Array<VideoMode> GetVideoModes() const;
        VideoMode GetVideoMode() const;
        GammaRamp GetGammaRamp() const;
        void SetGammaRamp(const GammaRamp& gammaRamp);
        void SetMonitor(GLFWmonitor* monitor);
        
    protected:
        GLFWmonitor* mMonitorGLFW = nullptr;
    };
}
#endif
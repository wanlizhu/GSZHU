#pragma once

#include "Core/BasicTypes.h"

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

    class DLLDECL Monitor
    {
        friend class Window;
    public:
        Monitor(GLFWmonitor* monitor = nullptr);

        GLFWmonitor* GetMonitor() const;
        String GetName() const;
        bool IsPrimary() const;
        glm::ivec2 GetClientAreaSize() const;
        glm::ivec2 GetClientAreaPos() const;
        glm::ivec2 GetPhysicalSize() const;
        glm::vec2 GetContentScale() const;
        glm::ivec2 GetPos() const;
        Array<VideoMode> GetVideoModes() const;
        VideoMode GetVideoMode() const;
        GammaRamp GetGammaRamp() const;
        void SetGammaRamp(const GammaRamp& gammaRamp);

    private:
        GLFWmonitor* mMonitorGLFW = nullptr;
    };
}
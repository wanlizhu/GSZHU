#ifdef PLATFORM_WINDOWS
#include "Application/Windows/Monitor.h"
#include "GLFW/glfw3.h"

namespace Wanli
{
    Monitor::Monitor(GLFWmonitor* monitor)
        : mMonitorGLFW(monitor)
    {}

    Monitor::~Monitor()
    {}

    Monitor::operator bool() const
    {
        return mMonitorGLFW != nullptr;
    }

    bool Monitor::IsPrimary() const
    {
        return mMonitorGLFW == glfwGetPrimaryMonitor();
    }

    GLFWmonitor* Monitor::GetMonitorGLFW() const
    {
        return mMonitorGLFW;
    }

    glm::ivec2 Monitor::GetClientAreaSize() const
    {
        int width;
        int height;
        glfwGetMonitorWorkarea(mMonitorGLFW, nullptr, nullptr, &width, &height);
        return { width, height };
    }

    glm::ivec2 Monitor::GetClientAreaPos() const
    {
        int xPos;
        int yPos;
        glfwGetMonitorWorkarea(mMonitorGLFW, &xPos, &yPos, nullptr, nullptr);
        return { xPos, yPos };
    }

    glm::ivec2 Monitor::GetPhysicalSize() const
    {
        int widthMM;
        int heightMM;
        glfwGetMonitorPhysicalSize(mMonitorGLFW, &widthMM, &heightMM);
        return { widthMM, heightMM };
    }

    glm::ivec2 Monitor::GetPos() const
    {
        int xpos;
        int ypos;
        glfwGetMonitorPos(mMonitorGLFW, &xpos, &ypos);
        return { xpos, ypos };
    }

    glm::vec2 Monitor::GetContentScale() const
    {
        float xScale;
        float yScale;
        glfwGetMonitorContentScale(mMonitorGLFW, &xScale, &yScale);
        return { xScale, yScale };
    }

    String Monitor::GetName() const
    {
        return glfwGetMonitorName(mMonitorGLFW);
    }

    Array<VideoMode> Monitor::GetVideoModes() const
    {
        int videoModeCount;
        auto videoModes = glfwGetVideoModes(mMonitorGLFW, &videoModeCount);
        Array<VideoMode> modes(videoModeCount);

        for (int i = 0; i < videoModeCount; i++)
        {
            modes[i] = *reinterpret_cast<const VideoMode*>(&videoModes[i]);
        }

        return modes;
    }

    VideoMode Monitor::GetVideoMode() const
    {
        auto videoMode = glfwGetVideoMode(mMonitorGLFW);
        return *reinterpret_cast<const VideoMode*>(videoMode);
    }

    GammaRamp Monitor::GetGammaRamp() const
    {
        auto gamaRamp = glfwGetGammaRamp(mMonitorGLFW);
        return *reinterpret_cast<const GammaRamp*>(gamaRamp);
    }

    void Monitor::SetGammaRamp(const GammaRamp& gammaRamp)
    {
        auto ramp = reinterpret_cast<const GLFWgammaramp*>(&gammaRamp);
        glfwSetGammaRamp(mMonitorGLFW, ramp);
    }
}

#endif
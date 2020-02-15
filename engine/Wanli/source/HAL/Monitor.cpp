#include "HAL/Monitor.h"
#include <glfw/glfw3.h>

namespace Wanli
{
    Monitor::Monitor(GLFWmonitor* monitor)
        : mMonitorGLFW(monitor)
    {}

    GLFWmonitor* Monitor::GetMonitor() const
    {
        return mMonitorGLFW;
    }

    String Monitor::GetName() const
    {
        return glfwGetMonitorName(mMonitorGLFW);
    }

    bool Monitor::IsPrimary() const
    {
        return mMonitorGLFW == glfwGetPrimaryMonitor();
    }

    glm::ivec2 Monitor::GetClientAreaSize() const
    {
        glm::ivec2 size;
        glfwGetMonitorWorkarea(mMonitorGLFW, nullptr, nullptr, &size.x, &size.y);
        return size;
    }

    glm::ivec2 Monitor::GetClientAreaPos() const
    {
        glm::ivec2 pos;
        glfwGetMonitorWorkarea(mMonitorGLFW, &pos.x, &pos.y, nullptr, nullptr);
        return pos;
    }

    glm::ivec2 Monitor::GetPhysicalSize() const
    {
        glm::ivec2 size;
        glfwGetMonitorPhysicalSize(mMonitorGLFW, &size.x, &size.y);
        return size;
    }

    glm::vec2 Monitor::GetContentScale() const
    {
        glm::vec2 scale;
        glfwGetMonitorContentScale(mMonitorGLFW, &scale.x, &scale.y);
        return scale;
    }

    glm::ivec2 Monitor::GetPos() const
    {
        glm::ivec2 pos;
        glfwGetMonitorPos(mMonitorGLFW, &pos.x, &pos.y);
        return pos;
    }

    Array<VideoMode> Monitor::GetVideoModes() const
    {
        int videoModeCount = 0;
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
        auto gammaRamp = glfwGetGammaRamp(mMonitorGLFW);
        return *reinterpret_cast<const GammaRamp*>(gammaRamp);
    }

    void Monitor::SetGammaRamp(const GammaRamp& gammaRamp)
    {
        auto ramp = reinterpret_cast<const GLFWgammaramp*>(&gammaRamp);
        glfwSetGammaRamp(mMonitorGLFW, ramp);
    }
}
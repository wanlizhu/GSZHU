#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

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

    struct Gamma
    {
        int16_t* red = nullptr;
        int16_t* green = nullptr;
        int16_t* blue = nullptr;
        int size = 0;
    };

    class IMonitor
    {
    public:
        virtual bool IsPrimary() const = 0;
        virtual glm::ivec2 GetDisplaySize() const = 0;
        virtual glm::ivec2 GetPhysicalSize() const = 0;
        virtual glm::vec2 GetScaleFactor() const = 0;
        virtual std::string GetName() const = 0;
        virtual std::vector<VideoMode> GetAvailableVideoModes() const = 0;
        virtual VideoMode GetVideoMode() const = 0;
        virtual Gamma GetGamma() const = 0;
        virtual void SetGamma(const Gamma& gamma) const = 0;
    };
}
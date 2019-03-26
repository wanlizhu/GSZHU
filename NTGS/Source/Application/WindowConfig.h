#pragma once

#include <string>
#include "Common/Vector.h"
#include "Common/ResourceFormat.h"

namespace NTGS {
    class WindowConfig {
    public:
        bool mHideWindow = false;
        bool mFullScreen = false;
        Vec2i mPosition = Vec2i(0, 0);
        Vec2i mSize = Vec2i(1024, 635);
        ColorFormat mColorFormat = RGB32F;
        ColorFormat mDepthStencilFormat = DEPTH32F;
        int mSampleCount = 1;
        int mSampleQuality = 0;
        int mSyncInterval = 0;
        bool mGamma = false;
        bool mKeepScreenOn = false;

        std::vector<std::pair<std::string, std::string>> mOptions;
    };
}
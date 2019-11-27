#pragma once

#import <Cocoa/Cocoa.h>
#include "Wanlix/Core3D/Platform/Display.h"

namespace Wanlix
{
    class MacOSDisplay : public Display 
    {
    public:
        using Ptr = std::shared_ptr<MacOSDisplay>;
        using ConstPtr = std::shared_ptr<const MacOSDisplay>;
        using UniquePtr = std::unique_ptr<MacOSDisplay>;

        static UniquePtr Create(CGDirectDisplayID displayId);
        ~MacOSDisplay();

        bool IsPrimary() const override;
        std::wstring GetDeviceName() const override;
        Offset GetOffset() const override;

        bool ResetDisplayMode() override;
        bool SetDisplayMode(const DisplayModeDescriptor& displayModeDesc) override;
        DisplayModeDescriptor GetDisplayMode() const override;
        std::vector<DisplayModeDescriptor> GetSupportedDisplayModes() const override;  
    
    private:
        MacOSDisplay(CGDirectDisplayID displayId);

    private:
        CGDirectDisplayID mDisplayId = 0;
        CGDisplayModeRef mDefaultDisplayModeRef = nullptr;
    };
}
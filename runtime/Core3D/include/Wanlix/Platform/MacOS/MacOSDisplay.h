#pragma once

#import <Cocoa/Cocoa.h>
#include "Wanlix/Platform/IDisplay.h"

namespace Wanlix
{
    class MacOSDisplay : public IDisplay 
    {
    public:
        using Ptr = std::shared_ptr<MacOSDisplay>;
        using ConstPtr = std::shared_ptr<const MacOSDisplay>;
        using UniquePtr = std::unique_ptr<MacOSDisplay>;

        static UniquePtr Create(CGDirectDisplayID displayId);
        ~MacOSDisplay();

        bool IsPrimary() const override;
        std::string GetDeviceName() const override;
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
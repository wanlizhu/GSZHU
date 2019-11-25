#pragma once

#include "Wanlix/Core/EngineConfig.h"
#include "Wanlix/Utility/NonCopyable.h"
#include "DisplayModeDescriptor.h"
#include <vector>
#include <memory>
#include <string>

namespace Wanlix
{
    class IDisplay : NonCopyable
    {
    public:
        using Ptr = std::shared_ptr<IDisplay>;
        using ConstPtr = std::shared_ptr<const IDisplay>;
        using UniquePtr = std::unique_ptr<IDisplay>;

        // Instantiates a list of interfaces for all connected displays.
        static std::vector<UniquePtr> List();
        // Instantiates an interface for the primary display.
        static UniquePtr Primary();
		// Shows or hides the cursor for the running application from all displays.
		static void ShowCursor();
		static void HideCursor();
		static bool IsCursorVisible();

        virtual bool IsPrimary() const = 0;
        virtual std::string GetDeviceName() const = 0;
        virtual Offset GetOffset() const = 0; // offset relative to the primary display
        
        virtual bool ResetDisplayMode() = 0;
        virtual bool SetDisplayMode(const DisplayModeDescriptor& desc) = 0;
        virtual DisplayModeDescriptor GetDisplayMode() const = 0;
        virtual std::vector<DisplayModeDescriptor> GetSupportedDisplayModes() const = 0;

    protected:
        // Sorts the specified list of display modes
        static void FinalizeDisplayModes(std::vector<DisplayModeDescriptor>& modes);
    };
}
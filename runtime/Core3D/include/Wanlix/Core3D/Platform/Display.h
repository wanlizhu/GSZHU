#pragma once

#include "Wanlix/Core3D/Types.h"
#include "Wanlix/Core3D/Utility/NonCopyable.h"

namespace Wanlix
{
    struct DisplayModeDescriptor
    {
        Extent   resolution;
        uint32_t refreshRate = 0; 

        Extent   GetExtentRatio() const;
        bool     operator==(const DisplayModeDescriptor& rhs) const;
        bool     operator< (const DisplayModeDescriptor& rhs) const;
    };

    class Display : NonCopyable
    {
    public:
        using Ptr       = std::shared_ptr<Display>;
        using ConstPtr  = std::shared_ptr<const Display>;
        using UniquePtr = std::unique_ptr<Display>;

        static std::vector<UniquePtr> List();
        static UniquePtr Primary();
		static void      ShowCursor();
		static void      HideCursor();
		static bool      IsCursorVisible();

        virtual bool    IsPrimary() const = 0;
        virtual WString GetDeviceName() const = 0;
        virtual Offset  GetOffset() const = 0; /* offset relative to the primary display */
        
        virtual bool ResetDisplayMode() = 0;
        virtual bool SetDisplayMode(const DisplayModeDescriptor& desc) = 0;

        virtual DisplayModeDescriptor              GetDisplayMode() const = 0;
        virtual std::vector<DisplayModeDescriptor> GetSupportedDisplayModes() const = 0;

    protected:
        // Sorts the specified list of display modes
        static void FinalizeDisplayModes(std::vector<DisplayModeDescriptor>& modes);
    };
}
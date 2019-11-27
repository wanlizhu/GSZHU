#pragma once

#include "Wanlix/Core3D/Platform/IDisplay.h"
#include "Win32LeanAndMean.h"
#include <Windows.h>

namespace Wanlix
{
    class Win32Display final : public IDisplay
    {
    public:
        using Ptr = std::shared_ptr<Win32Display>;
        using ConstPtr = std::shared_ptr<const Win32Display>;
        using UniquePtr = std::unique_ptr<Win32Display>;

        static UniquePtr Create(HMONITOR monitor);

        bool IsPrimary() const override;
        std::wstring GetDeviceName() const override;
        Offset GetOffset() const override;

        bool ResetDisplayMode() override;
        bool SetDisplayMode(const DisplayModeDescriptor& displayModeDesc) override;
        DisplayModeDescriptor GetDisplayMode() const override;
        std::vector<DisplayModeDescriptor> GetSupportedDisplayModes() const override;

    private:
        Win32Display(HMONITOR monitor);
        void GetInfo(MONITORINFO& info) const;
        void GetInfo(MONITORINFOEX& info) const;

    private:
        HMONITOR mMonitor = nullptr;
    };
}
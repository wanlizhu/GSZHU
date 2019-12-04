#pragma once

#include "Wanlix/PAL/Display.h"
#include <Windows.h>

namespace Wanlix
{
    class Win32Display final : public Display
    {
    public:
        using SharedPtr = std::shared_ptr<Win32Display>;
        using UniquePtr = std::unique_ptr<Win32Display>;

        static UniquePtr Create(HMONITOR monitor);

        std::wstring GetDeviceName() const override;
        bool IsPrimary() const override;
        INT2 GetOffset() const override;

        bool ResetDisplayMode() override;
        bool SetDisplayMode(const DisplayMode& displayModeDesc) override;

        DisplayMode              GetDisplayMode() const override;
        std::vector<DisplayMode> GetSupportedDisplayMode() const override;

    private:
        Win32Display(HMONITOR monitor);
        void GetInfo(MONITORINFO& info) const;
        void GetInfo(MONITORINFOEX& info) const;

    private:
        HMONITOR mMonitor = nullptr;
    };
}
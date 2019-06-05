#pragma once

#include <GSZHU/IEvent.h>
#include <GSZHU/EKeyCode.h>

namespace GSZHU {
    class CKeyEvent : public IEvent {
    public:
        CKeyEvent(EKeyCode code, uint32_t unicode, bool isDown,
                  bool shift = false, bool ctrl = false)
            : mKeyCode(code)
            , mUnicode(unicode)
            , mIsDown(isDown)
            , mShift(shift)
            , mCtrl(ctrl)
        {}

        virtual EType GetType() const override { return EType::Key; }
        EKeyCode GetKeyCode() const { return mKeyCode; }
        uint32_t GetUnicode() const { return mUnicode; }
        bool IsDown() const { return mIsDown; }
        bool HasShift() const { return mShift; }
        bool HasCtrl() const { return mCtrl; }

    private:
        EKeyCode mKeyCode = EKeyCode::None;
        uint32_t mUnicode = 0;
        bool mIsDown = false;
        bool mShift = false;
        bool mCtrl = false;
    };
}
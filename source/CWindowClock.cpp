#include "CWindowClock.h"
#include <assert.h>

namespace GSZHU {
    std::unordered_map<CWindowClock::EUnit, double> sUnitRates = {
        { CWindowClock::EUnit::NanoSecond, 1.0 },
        { CWindowClock::EUnit::MicroSecond, 1e-3 },
        { CWindowClock::EUnit::MilliSecond, 1e-6 },
        { CWindowClock::EUnit::Second, 1e-9 }
    };

    CWindowClock::CWindowClock() {
        mClockTable[EType::UpdateClock] = SClock();
        mClockTable[EType::RenderClock] = SClock();
        ResetAll();
    }

    CWindowClock::~CWindowClock() {
        ResetAll();
        mClockTable.clear();
    }

    void CWindowClock::Tick(EType type) {
        assert(mClockTable.find(type) != mClockTable.end());
        auto now = std::chrono::high_resolution_clock::now();
        auto& clock = mClockTable[type];
        
        clock.deltaTime = now - clock.lastTick;
        clock.totalTime += clock.deltaTime;
        clock.lastTick = now;
    }

    void CWindowClock::Reset(EType type) {
        assert(mClockTable.find(type) != mClockTable.end());
        auto now = std::chrono::high_resolution_clock::now();
        auto& clock = mClockTable[type];

        clock.deltaTime = std::chrono::high_resolution_clock::duration();
        clock.totalTime = std::chrono::high_resolution_clock::duration();
        clock.lastTick = now;
    }

    void CWindowClock::ResetAll() {
        for (auto& iter = mClockTable.begin(); iter != mClockTable.end(); iter++) {
            Reset(iter->first);
        }
    }

    double CWindowClock::GetDelta(EType type, EUnit unit) const {
        return mClockTable.at(type).deltaTime.count() * sUnitRates[unit];
    }

    double CWindowClock::GetTotal(EType type, EUnit unit) const {
        return mClockTable.at(type).totalTime.count() * sUnitRates[unit];
    }
}
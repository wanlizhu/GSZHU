#pragma once

#include <chrono>
#include <array>
#include <unordered_map>

namespace GSZHU {
    class CWindowClock {
    public:
        enum EType : int {
            UpdateClock,
            RenderClock,
        };
        enum EUnit : int {
            Second,
            MilliSecond,
            MicroSecond,
            NanoSecond
        };

        CWindowClock();
        virtual ~CWindowClock();

        void Tick(EType type);
        void Reset(EType type);
        void ResetAll();
        double GetDelta(EType type, EUnit unit = EUnit::MilliSecond) const;
        double GetTotal(EType type, EUnit unit = EUnit::MilliSecond) const;

    private:
        struct SClock {
            std::chrono::high_resolution_clock::time_point lastTick;
            std::chrono::high_resolution_clock::duration deltaTime;
            std::chrono::high_resolution_clock::duration totalTime;
            SClock() : deltaTime(0), totalTime(0) {}
        };
        std::unordered_map<EType, SClock> mClockTable;
    };
}
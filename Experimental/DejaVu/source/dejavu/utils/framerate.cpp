#include "framerate.h"

namespace djv
{
    void Framerate::reset()
    {
        mFrameCount = 0;
        mFrameWindow = 60;
        mFrameTimes.clear();
    }

    void Framerate::tick()
    {
        mFrameCount++;
        mFrameTimes[mFrameCount % mFrameWindow] = mClock.tick().getDelta();
    }

    void Framerate::setFrameWindow(uint64_t count)
    {
        mFrameWindow = count;
    }

    uint64_t Framerate::getFrameCount() const
    {
        return mFrameCount;
    }

    Clock::duration Framerate::getAverageFrameTime() const
    {
        uint64_t frames = std::min(mFrameCount, mFrameWindow);
        Clock::duration elapsedTime = Clock::duration(0);

        for (uint64_t i = 0; i < frames; i++) {
            elapsedTime += mFrameTimes[i];
        }

        return elapsedTime / frames;
    }

    Clock::duration Framerate::getLastFrameTime() const
    {
        return mFrameTimes[mFrameCount % mFrameWindow];
    } 
}
#pragma once

#include "BasicTypes.h"
#include "Utilities/NonCopyable.h"
#include "Utilities/Delegate.h"

namespace Wanli
{
    enum class EFileState
    {
        Created,
        Modified,
        Removed,
    };

    class DLLDECL FileObserver : public NonCopyable
    {
    public:
        FileObserver(const Path& path, const Second& delay);
        ~FileObserver();

        inline const Path& GetPath() const { return mPath; }
        inline const Second& GetDelay() const { return mDelay; }
        inline void SetPath(const Path& path) { mPath = path; }
        inline void SetDelay(const Second& delay) { mDelay = delay; }
        inline Delegate<void(const Path&, EFileState)>& OnChange() { return mOnChangeDelegate; }

    private:
        void ThreadFunc();
        
    private:
        Path mPath;
        Second mDelay;
        Delegate<void(const Path&, EFileState)> mOnChangeDelegate;
        HashMap<String, FileTime> mLastModifiedTimes;
    };
}
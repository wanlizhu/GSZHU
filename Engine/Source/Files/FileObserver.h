#pragma once

#include "Core/Common.h"
#include "Utils/NonCopyable.h"
#include "Utils/Delegate.h"

namespace Wanli
{
    enum class EFileState
    {
        Created,
        Modified,
        Removed,
    };

    class FileObserver : public NonCopyable
    {
    public:
        FileObserver(const Path& path, const Duration_SEC& delay);
        ~FileObserver();

        Path const& GetPath() const;
        Duration_SEC const& GetDelay() const;
        void SetPath(const Path& path);
        void SetDelay(const Duration_SEC& delay);
        Delegate<void(const Path&, EFileState)>& OnChange();

    private:
        void ThreadFunc();
        
    private:
        Path mPath;
        Duration_SEC mDelay;
        Delegate<void(const Path&, EFileState)> mOnChangeDelegate;
        HashMap<String, FileTime> mLastModifiedTimes;
    };
}
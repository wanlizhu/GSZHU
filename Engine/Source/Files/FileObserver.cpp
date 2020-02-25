#include "FileObserver.h"
#include "Files/File.h"

namespace Wanli
{
    FileObserver::FileObserver(const Path& path, const Duration_SEC& delay)
        : mPath(path)
        , mDelay(delay)
    {}

    FileObserver::~FileObserver()
    {}

    Path const& FileObserver::GetPath() const 
    { 
        return mPath;
    }

    Duration_SEC const& FileObserver::GetDelay() const
    { 
        return mDelay;
    }

    void FileObserver::SetPath(const Path& path) 
    {
        mPath = path;
    }

    void FileObserver::SetDelay(const Duration_SEC& delay) 
    {
        mDelay = delay; 
    }

    Delegate<void(const Path&, EFileState)>& FileObserver::OnChange() 
    {
        return mOnChangeDelegate;
    }

    void FileObserver::ThreadFunc()
    {}
}
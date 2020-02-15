#ifdef PLATFORM_WINDOWS
#include "HAL/File.h"
#include "Utilities/String.h"
#include "Utilities/Log.h"
#include "Core/Engine.h"
#include <Windows.h>

namespace Wanli
{
    Array<Path> File::smSearchPath;

    class File::FoundData : public NonCopyable
        , public std::enable_shared_from_this<File::FoundData>
    {
    public:
        Path baseDirectory;
        WIN32_FIND_DATA findData = {};
        HANDLE findHandle = NULL;

    public:
        virtual ~FoundData()
        {
            if (findHandle)
            {
                FindClose(findHandle);
                findHandle = NULL;
            }
        }
    };

    File::Enumerate::Enumerate(const Path& directory, const wchar_t* filter)
        : mDirectoryPath(directory)
        , mFilter(filter)
    {}

    File File::Enumerate::begin() const
    {
        Path finalPath = mDirectoryPath / mFilter;
        SharedPtr<FoundData> foundData = std::make_shared<FoundData>();

        foundData->baseDirectory = mDirectoryPath;
        foundData->findHandle = FindFirstFileW(finalPath.wstring().c_str(), &(foundData->findData));
        if (foundData->findHandle != INVALID_HANDLE_VALUE)
        {
            return ++File(foundData);
        }
        return File();
    }

    File File::Enumerate::end() const
    {
        return File();
    }

    void File::AddSearchPath(const Path& path)
    {
        if (std::find(smSearchPath.begin(), smSearchPath.end(), path) == smSearchPath.end())
        {
            smSearchPath.push_back(path);
        }
    }

    void File::ClearSearchPath()
    {
        smSearchPath.clear();
    }

    bool File::FindFile(const String& filename, Path* absolutePath, bool recursive)
    {
        return true;
    }

    File::File()
    {}

    File::File(const Path& path)
        : mFilePath(path)
    {}

    File::File(SharedPtr<FoundData> foundData)
        : mFoundData(foundData)
    {}

    File::File(const Path& path, SharedPtr<FoundData> foundData)
        : mFilePath(path)
        , mFoundData(foundData)
    {}

    File::~File() 
    {}

    size_t File::GetSize() const
    {
        size_t size = 0;

        return size;
    }

    UniquePtr<ByteArray> File::ReadFile() const
    {
        UniquePtr<ByteArray> result;

        return result;
    }

    void File::WriteFile(const void* data, size_t size)
    {

    }

    File& File::operator++()
    {
        if (mFoundData)
        {
            *this = File();
            while (FindNextFileW(mFoundData->findHandle, &(mFoundData->findData)))
            {
                std::wstring fileName = mFoundData->findData.cFileName;
                if (fileName == L"." || fileName == L"..")
                    continue;

                *this = File(mFoundData->baseDirectory / fileName, mFoundData);
                break;
            }
        }

        return *this;
    }

    const File File::operator++(int)
    {
        File tmp(*this);
        ++(*this);
        return tmp;
    }

    File::operator bool() const
    {
        return !mFilePath.empty() || mFoundData;
    }
}
#endif
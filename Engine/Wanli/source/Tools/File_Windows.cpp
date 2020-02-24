#ifdef PLATFORM_WINDOWS
#include "Utilities/File.h"
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

    File::Enumerate::Enumerate(const Path& directory,
                               const std::wstring& filter)
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
            return File(foundData->baseDirectory / foundData->findData.cFileName,
                        foundData);
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

    bool File::FindFileInSearchPath(const std::wstring& filename,
                                    Path* absolutePath,
                                    bool recursive)
    {
        for (const auto& root : smSearchPath)
        {
            File file = File::Enumerate(root, L"*/" + filename).begin();
            if (file)
            {
                if (absolutePath)
                {
                    *absolutePath = file.GetFilePath();
                }
                return true;
            }
        }
        return false;
    }

    File::File()
    {}

    File::File(const Path& path, SharedPtr<FoundData> foundData)
        : mFilePath(path)
        , mFoundData(foundData)
    {}

    File::~File() 
    {}

    size_t File::GetSize() const
    {
        WIN32_FIND_DATAW fileInfo;
        HANDLE handle = NULL;
        DWORD64 size = 0;

        handle = ::FindFirstFileW(mFilePath.wstring().c_str(), &fileInfo);
        if (handle != INVALID_HANDLE_VALUE)
        {
            size = ((DWORD64)fileInfo.nFileSizeHigh << 32) + fileInfo.nFileSizeLow; 
        }
        ::FindClose(handle);

        return size;
    }

    UniquePtr<ByteArray> File::ReadFile() const
    {
        HANDLE file = ::CreateFileW(mFilePath.wstring().c_str(),
                                    GENERIC_READ,
                                    FILE_SHARE_READ,
                                    NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL,
                                    NULL);
        if (file == INVALID_HANDLE_VALUE)
        {
            LOG_ERROR("!!! [error: %d] Failed to open file !!!\n", ::GetLastError());
            return nullptr;
        }

        UniquePtr<ByteArray> result;

        do 
        {
            size_t size = GetSize();
            if (size > MAXDWORD)
            {
                LOG_ERROR("!!! Use ReadFileAsync() for big file (size > 4GB) !!!\n");
                break;
            }

            result = std::make_unique<ByteArray>(size);
            DWORD readBytes = (DWORD)result->size();

            if (!::ReadFile(file, result->data(), readBytes, &readBytes, NULL))
            {
                LOG_ERROR("!!! [error: %d] Failed to read file !!!\n", ::GetLastError());
                result = nullptr;
                break;
            }
        } while (false);

        CloseHandle(file);
        return result;
    }

    Optional<Future<UniquePtr<ByteArray>>> File::ReadFileAsync() const
    {
        return std::nullopt;
    }

    bool File::WriteFile(const void* data, Uint size)
    {
        HANDLE file = ::CreateFileW(mFilePath.wstring().c_str(),
                                    GENERIC_WRITE,
                                    FILE_SHARE_READ,
                                    NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL,
                                    NULL);
        if (file == INVALID_HANDLE_VALUE)
        {
            LOG_ERROR("!!! [error: %d] Failed to open file !!!\n", ::GetLastError());
            return false;
        }

        DWORD writeBytes = (DWORD)size;
        BOOL success = ::WriteFile(file, data, writeBytes, &writeBytes, NULL);
    
        return success == TRUE && writeBytes == size;
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
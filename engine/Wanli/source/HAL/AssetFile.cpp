#include "HAL/AssetFile.h"
#include "physfs.h"
#include <assert.h>
#include "Utilities/Log.h"

namespace Wanli
{
    enum class EFileMode
    {
        Read,
        Write,
        Append,
    };

    PHYSFS_File* OpenFile(const char* filename, EFileMode mode)
    {
        PHYSFS_File* file = nullptr;

        switch (mode)
        {
            case EFileMode::Read: file = PHYSFS_openRead(filename); break;
            case EFileMode::Write: file = PHYSFS_openWrite(filename); break;
            case EFileMode::Append: file = PHYSFS_openAppend(filename); break;
            default: assert(false);
        }
        
        if (file == nullptr)
        {
            LOG_WARNING("[%s] Failed to open file: '%s'.\n", 
                        PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()),
                        filename);
        }

        return file;
    }

    void CloseFile(PHYSFS_File* file)
    {
        if (PHYSFS_close(file) == 0)
        {
            LOG_WARNING("[%s] Failed to close file.\n", 
                        PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        }
    }

    AssetFile::AssetFile(const String& name)
        : mFileName(name)
    {
        std::replace(mFileName.begin(), mFileName.end(), '\\', '/');
    }

    AssetFile::~AssetFile()
    {}

    bool AssetFile::IsDirectory() const
    {
        return PHYSFS_isDirectory(mFileName.c_str());
    }

    bool AssetFile::IsSymbolicLink() const
    {
        return PHYSFS_isSymbolicLink(mFileName.c_str());
    }

    size_t AssetFile::FileSize() const
    {
        PHYSFS_File* file = OpenFile(mFileName.c_str(), EFileMode::Read);
        size_t size = PHYSFS_fileLength(file);
        CloseFile(file);
        return size;
    }

    void AssetFile::AppendFile(const void* data, size_t size) const
    {
        PHYSFS_File* file = OpenFile(mFileName.c_str(), EFileMode::Append);
        PHYSFS_writeBytes(file, data, size);
        CloseFile(file);
    }

    void AssetFile::WriteFile(const void* data, size_t size) const
    {
        PHYSFS_File* file = OpenFile(mFileName.c_str(), EFileMode::Write);
        PHYSFS_writeBytes(file, data, size);
        CloseFile(file);
    }

    SharedPtr<ByteArray> AssetFile::ReadFile() const
    {
        PHYSFS_File* file = OpenFile(mFileName.c_str(), EFileMode::Read);
        auto size = PHYSFS_fileLength(file);
        SharedPtr<ByteArray> data = std::make_shared<ByteArray>(size);
        PHYSFS_readBytes(file, data->data(), size);
        CloseFile(file);
        return data;
    }
}
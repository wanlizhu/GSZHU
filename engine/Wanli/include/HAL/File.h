#pragma once

#include <string>
#include <fstream>
#include "BasicTypes.h"
#include "Utilities/NonCopyable.h"

struct PHYSFS_File;

namespace Wanli
{
    class DLLDECL File : public NonCopyable
    {
        friend class FileManager;
    public:
        virtual ~File();

        bool IsDirectory() const;
        bool IsSymbolicLink() const;

        size_t FileSize() const;
        void AppendFile(const void* data, size_t size) const;
        void WriteFile(size_t offset, const void* data, size_t size) const;
        SharedPtr<ByteArray> ReadFile(size_t offset = 0, size_t size = (size_t)-1) const;
        
    protected:
        File(const String& name);

    private:
        String mFileName;
    };
}
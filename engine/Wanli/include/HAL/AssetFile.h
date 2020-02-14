#pragma once

#include <string>
#include <fstream>
#include "BasicTypes.h"
#include "Utilities/NonCopyable.h"

struct PHYSFS_File;

namespace Wanli
{
    class DLLDECL AssetFile : public NonCopyable
    {
        friend class AssetManager;
    public:
        AssetFile(const String& name);
        virtual ~AssetFile();

        bool IsDirectory() const;
        bool IsSymbolicLink() const;

        size_t FileSize() const;
        void AppendFile(const void* data, size_t size) const;
        void WriteFile(const void* data, size_t size) const;
        SharedPtr<ByteArray> ReadFile() const;
        
    private:
        String mFileName;
    };
}
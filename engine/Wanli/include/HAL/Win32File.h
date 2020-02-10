#pragma once

#include "HAL/IFile.h"

namespace Wanli
{
    class DLLDECL Win32File final : public IFile
    {
    public:
        virtual ~Win32File();
        virtual bool IsOpen() const override final;
        virtual bool IsDirectory() const override final;
        virtual bool IsSymbolicLink() const override final;
        virtual bool IsEmpty() const override final;

        virtual size_t FileSize() const override final;
        virtual Path FilePath() const override final;
        virtual void AppendFile(const void* data, size_t size) const override final;
        virtual void WriteFile(size_t offset, const void* data, size_t size) const override final;
        virtual SharedPtr<ByteArray> ReadFile(size_t offset = 0, size_t size = (size_t)-1) const override final;
        virtual SharedPtr<StringArray> FilesList(bool recursive = false) const override final;

        virtual void* CreateMemoryMap() override final;
        virtual void FlushMemoryMap() const override final;
        virtual void CloseMemoryMap() override final;
    };
}
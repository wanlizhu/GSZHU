#pragma once

#include "Core/BasicTypes.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    class DLLDECL File : public std::enable_shared_from_this<File>
    {
    public:
        class FoundData;
        class Enumerate
        {
        public:
            Enumerate(const Path& directory, const wchar_t* filter = L"*");
            
            File begin() const;
            File end() const;

        private:
            Path mDirectoryPath;
            const wchar_t* mFilter = nullptr;
        };

        static void AddSearchPath(const Path& path);
        static void ClearSearchPath();
        static bool FindFile(const String& filename, Path* absolutePath, bool recursive = true);

        File();
        explicit File(const Path& path);
        explicit File(SharedPtr<FoundData> foundData);
        File(const Path& path, SharedPtr<FoundData> foundData);
        virtual ~File();
        
        size_t GetSize() const;
        UniquePtr<ByteArray> ReadFile() const;
        void WriteFile(const void* data, size_t size);
        inline Path GetFilePath() const { return mFilePath; }

        File& operator++();
        const File operator++(int);

        operator bool() const;

    protected:
        static Array<Path> smSearchPath;
        Path mFilePath;
        SharedPtr<FoundData> mFoundData;
    };
}
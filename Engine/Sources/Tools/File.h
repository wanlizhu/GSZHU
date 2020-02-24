#pragma once

#include "Core/BasicTypes.h"
#include "Utilities/NonCopyable.h"
#include "Utilities/Future.h"

namespace Wanli
{
    class File : public std::enable_shared_from_this<File>
    {
    public:
        class FoundData;
        class Enumerate
        {
        public:
            Enumerate(const Path& directory,
                      const std::wstring& filter = L"*");
            
            File begin() const;
            File end() const;

        private:
            Path mDirectoryPath;
            std::wstring mFilter;
        };

        static void AddSearchPath(const Path& path);
        static void ClearSearchPath();
        static bool FindFileInSearchPath(const std::wstring& filename,
                                         Path* absolutePath,
                                         bool recursive = true);

        File();
        File(const Path& path, SharedPtr<FoundData> foundData = nullptr);
        virtual ~File();
        
        size_t GetSize() const;
        bool WriteFile(const void* data, Uint size);
        UniquePtr<ByteArray> ReadFile() const;
        Optional<Future<UniquePtr<ByteArray>>> ReadFileAsync() const;
        
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
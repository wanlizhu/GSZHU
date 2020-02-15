#pragma once

#include "Core/BasicTypes.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    template<typename _Base_>
    class DLLDECL BitmapFactory
    {
    public:
        using LoadMethod = std::function<void(_Base_*, const String&)>;
        using WriteMethod = std::function<void(const _Base_*, const String&)>;
        
        struct RegistryEntry 
        {
            LoadMethod loadMethod;
            WriteMethod writeMethod;
        };

        template<typename T>
        class Registrar //: public _Base_
        {
        public:
            template<typename... _Args_>
            static bool Register(_Args_&&... names)
            {
                for (String&& name : { names ... })
                {
                    BitmapFactory::Registry()[name].loadMethod = &T::Load;
                    BitmapFactory::Registry()[name].writeMethod = &T::Write;
                }
                return true;
            }
        };

    public:
        static HashMap<String, RegistryEntry>& Registry()
        {
            static HashMap<String, RegistryEntry> instance;
            return instance;
        }
    };


    class DLLDECL Bitmap : public NonCopyable
        , public std::enable_shared_from_this<Bitmap>
        , public BitmapFactory<Bitmap>
    {
    public:
        Bitmap() = default;
        explicit Bitmap(String path);
        explicit Bitmap(const glm::ivec2& size,
                        int bytesPerPixel = 4);
        explicit Bitmap(const glm::ivec2& size,
                        int bytesPerPixel,
                        UniquePtr<Byte[]>&& data);
        virtual ~Bitmap() = default;

        void Load(const String& path);
        void Write(const String& path) const;

        inline operator bool() const noexcept { return !mData; }
        inline Uint GetSizeInBytes() const { return mSize.x * mSize.y * mBytesPerPixel; }
        
        inline String GetPath() const { return mFilePath; }
        inline void SetPath(const String& path) { mFilePath = path; }
        
        inline const UniquePtr<Byte[]>& GetData() const { return mData; }
        inline UniquePtr<Byte[]>& GetData() { return mData; }
        inline void SetData(UniquePtr<Byte[]>&& data) { mData = std::move(data); }

        inline const glm::ivec2& GetSize() const { return mSize; }
        inline void SetSize(const glm::ivec2& size) { mSize = size; }

        inline int GetBytesPerPixel() const { return mBytesPerPixel; }
        inline void SetBytesPerPixel(int bytes) { mBytesPerPixel = bytes; }

    protected:
        String mFilePath;
        UniquePtr<Byte[]> mData;
        glm::ivec2 mSize = { 0, 0 };
        int mBytesPerPixel = 4;
    };
}
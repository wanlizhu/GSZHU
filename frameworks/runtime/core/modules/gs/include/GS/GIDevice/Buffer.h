#pragma once

#include "ResourcePool.h"

namespace GS::GI
{
    class Buffer : public ResourceNoWait
    {
    public:
        using Ptr = std::shared_ptr<Buffer>;

        Buffer(const char* name,
               Device::Ptr device,
               size_t bytes,
               const void* data = nullptr);

        // virtual methods from `ResourceNoWait`
        virtual void Flush() override;
        virtual void Init() override;
        virtual void Destroy() override;
        virtual operator bool() const override;

        Range  GetRange() const;
        size_t ExpandBy(size_t bytes);
        size_t Append(const void* data, size_t bytes);
        size_t Update(const Range& range, const void* data);
        size_t Read(const Range& range, void* data) const;

        void*  Map(const Range& range);
        void*  CoherentMap(const Range& range);
        void   Unmap(void* data);
        bool   IsMapped(const Range& range);

    protected:
        std::unordered_map<void*, Range> mMappedRanges;
    };

    template<typename T>
    class ArrayBuffer : public Buffer
    {
    public:
        using Ptr = std::shared_ptr<ArrayBuffer<T>>;
        using Element = T;
        
        ArrayBuffer(const char* name,
                    Device::Ptr device,
                    size_t bytes,
                    const void* data = nullptr);
        
        bool IsOpen() const;
        bool IsAlignedByElement() const;
        bool Open();  // make and store a coherent mapped pointer for device buffer
        void Close(); // unmap for device buffer

        size_t GetNumElements() const;
        size_t GetNumElementsUsed() const;
        size_t GetNumElementsFree() const;

        const T& operator[](size_t i) const;
        T&       operator[](size_t i);

    protected:
        void* mMappedData = nullptr;
    };

    template<typename _Vertex_>
    class VertexBuffer : public ArrayBuffer<_Vertex_> 
    {
    public:
        using Ptr = std::shared_ptr<VertexBuffer<_Vertex_>>;

        VertexBuffer(const char* name,
                     Device::Ptr device,
                     size_t bytes,
                     const void* data = nullptr);
    };

    template<typename _Index_>
    class IndexBuffer : public ArrayBuffer<_Index_>
    {
    public:
        using Ptr = std::shared_ptr<IndexBuffer<_Index_>>;

        IndexBuffer(const char* name,
                    Device::Ptr device,
                    size_t bytes,
                    const void* data = nullptr);
    };

    template<typename T>
    class UniformBuffer : public ArrayBuffer<T>
    {
    public:
        using Ptr = std::shared_ptr<IndexBuffer<T>>;

        UniformBuffer(const char* name,
                      Device::Ptr device,
                      size_t bytes,
                      const void* data = nullptr);
    };
}
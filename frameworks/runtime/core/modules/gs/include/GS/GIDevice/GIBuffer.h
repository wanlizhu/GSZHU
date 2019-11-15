#pragma once

#include "GIResource.h"

namespace GS
{
    class GIBuffer : public GIResource
    {
    public:
        using Ptr = std::shared_ptr<GIBuffer>;
        using ConstPtr = std::shared_ptr<const GIBuffer>;

        virtual void Flush() override;
        virtual void Init(void* data) override;
        virtual void Destroy() override;

        size_t ExpandBy(size_t bytes);
        size_t Append(const void* data, size_t bytes);

        void* Map(const GIRange& range);
        void* CoherentMap(const GIRange& range);
        void Unmap(void* data);
        bool IsMapped(const GIRange& range);

    protected:
        GIBuffer(const std::string& name,
                 GIDevice::Ptr device,
                 size_t bytes,
                 const void* data = nullptr,
                 uint32_t flags = 0);

    protected:
        std::unordered_map<void*, GIRange> mMappedRanges;
    };

    template<typename T>
    class GIArrayBuffer : public GIBuffer
    {
    public:
        using Ptr = std::shared_ptr<GIArrayBuffer<T>>;
        using ConstPtr = std::shared_ptr<const GIArrayBuffer<T>>;
        using Element = T;
        
        static Ptr Create(const std::string& name,
                          GIDevice::Ptr device,
                          size_t bytes,
                          const void* data = nullptr);
        
        bool IsAlignedByElement() const;
        size_t GetNumElements() const;
        size_t GetNumElementsUsed() const;
        size_t GetNumElementsFree() const;

        const T& operator[](size_t i) const;
        T& operator[](size_t i);

    protected:
        GIArrayBuffer(const std::string& name,
                      GIDevice::Ptr device,
                      size_t bytes,
                      const void* data = nullptr);

    protected:
        void* mMappedData = nullptr;
    };

    template<typename _Vertex_>
    class GIVertexBuffer : public GIArrayBuffer<_Vertex_> 
    {
    public:
        using Ptr = std::shared_ptr<GIVertexBuffer<_Vertex_>>;
        using ConstPtr = std::shared_ptr<const GIVertexBuffer<_Vertex_>>;

        static Ptr Create(const std::string& name,
                          GIDevice::Ptr device,
                          size_t bytes,
                          const void* data = nullptr);

    protected:
        GIVertexBuffer(const std::string& name,
                       GIDevice::Ptr device,
                       size_t bytes,
                       const void* data = nullptr);
    };

    template<typename _Index_>
    class GIIndexBuffer : public GIArrayBuffer<_Index_>
    {
    public:
        using Ptr = std::shared_ptr<GIIndexBuffer<_Index_>>;
        using ConstPtr = std::shared_ptr<const GIIndexBuffer<_Index_>>;

        static Ptr Create(const std::string& name,
                          GIDevice::Ptr device,
                          size_t bytes,
                          const void* data = nullptr);

    protected:
        GIIndexBuffer(const std::string& name,
                      GIDevice::Ptr device,
                      size_t bytes,
                      const void* data = nullptr);
    };

    template<typename T>
    class GIUniformBuffer : public GIArrayBuffer<T>
    {
    public:
        using Ptr = std::shared_ptr<GIUniformBuffer<T>>;
        using ConstPtr = std::shared_ptr<const GIUniformBuffer<T>>;

        static Ptr Create(const std::string& name,
                          GIDevice::Ptr device,
                          size_t bytes,
                          const void* data = nullptr);

    protected:
        GIUniformBuffer(const std::string& name,
                        GIDevice::Ptr device,
                        size_t bytes,
                        const void* data = nullptr);
    };

    template<typename T>
    class GIShaderWritableBuffer : public GIArrayBuffer<T>
    {
    public:
        using Ptr = std::shared_ptr<GIShaderWritableBuffer<T>>;
        using ConstPtr = std::shared_ptr<const GIShaderWritableBuffer<T>>;

        static Ptr Create(const std::string& name,
                          GIDevice::Ptr device,
                          size_t bytes,
                          const void* data = nullptr);

    protected:
        GIShaderWritableBuffer(const std::string& name,
                               GIDevice::Ptr device,
                               size_t bytes,
                               const void* data = nullptr);
    };
}
#pragma once

#include <NTGS/Graphics/IBuffer.h>
#include <NTGS/Core/Utilities.h>
#include <NTGS/Graphics/IDeviceContext.h>

namespace NTGS {
    template<typename DataType>
    class MappedData : public NonCopyable {
    public:
        MappedData() = default;
        MappedData(IDeviceContext* Context, IBuffer* Buffer, MAP_TYPE MapType, MAP_FLAGS MapFlags)
            : MappedData() {
            Map(Context, Buffer, MapType, MapFlags);
        }
        MappedData(MappedData&& Other)
            : m_Buffer(std::move(Other.m_Buffer))
            , m_MappedData(std::move(Other.m_MappedData))
            , m_Context(std::move(Other.m_Context))
            , m_MapType(std::move(Other.m_MapType))
            , m_MapFlags(std::move(Other.m_MapFlags)) {
            Other.m_Buffer = nullptr;
            Other.m_Context = nullptr;
            Other.m_MappedData = nullptr;
            Other.m_MapType = static_cast<MAP_TYPE>(-1);
            Other.m_MapFlags = static_cast<Uint32>(-1);
        }
        MappedData& operator=(MappedData&& Other) {
            m_Buffer = std::move(Other.m_Buffer);
            m_MappedData = std::move(Other.m_MappedData);
            m_Context = std::move(Other.m_Context);
            m_MapType = std::move(Other.m_MapType);
            m_MapFlags = std::move(Other.m_MapFlags);
            Other.m_Buffer = nullptr;
            Other.m_Context = nullptr;
            Other.m_MappedData = nullptr;
            Other.m_MapType = static_cast<MAP_TYPE>(-1);
            Other.m_MapFlags = static_cast<Uint32>(-1);
            return *this;
        }
        virtual ~MappedData() { Unmap(); }

        inline bool Map(IDeviceContext* Context, IBuffer* Buffer, MAP_TYPE MapType, MAP_FLAGS MapFlags) {
            CHECK(!m_Buffer && m_MappedData && !m_Context, "Object already mapped");
            Unmap();

#ifdef _DEBUG
            auto& BufferDesc = Buffer->GetDesc();
            CHECK(sizeof(DataType) <= BufferDesc.SizeInBytes, "Data type size exceeds buffer size");
#endif

            m_MappedData = Context->MapBuffer(Buffer, MapType, MapFlags);
            if (m_MappedData != nullptr) {
                m_Context = Context;
                m_Buffer = Buffer;
                m_MapType = MapType;
                m_MapFlags = MapFlags;
                return true;
            } 
            else
                return false;
        }

        inline void Unmap() {
            if (m_Buffer) {
                m_Context->UnmapBuffer(m_Buffer, m_MapType);
                m_Buffer = nullptr;
                m_MapType = static_cast<MAP_TYPE>(-1);
                m_MapFlags = static_cast<UINT>(-1);
            }
            m_Context = nullptr;
            m_MappedData = nullptr;
        }

        inline operator DataType*() { return m_MappedData; }
        inline operator const DataType*() const { return m_MappedData; }
        DataType* operator->() { return m_MappedData; }
        const DataType* operator->() const { return m_MappedData; }

    private:
        IBuffer* m_Buffer = nullptr;
        IDeviceContext* m_Context = nullptr;
        DataType* m_MappedData = nullptr;
        MAP_TYPE m_MapType = static_cast<MAP_TYPE>(-1);
        UINT m_MapFlags = static_cast<UINT>(-1);
    };
}
#pragma once

#include "IDeviceContext.h"
#include "IBuffer.h"

namespace Wanlix
{
    template<typename T, Bool _StrongRef_ = false>
    class MappedData final : public NonCopyable
    {
        using ContextPtr = std::conditional_t<_StrongRef_, std::shared_ptr<IDeviceContext>, IDeviceContext*>;
        using BufferPtr  = std::conditional_t<_StrongRef_, std::shared_ptr<IBuffer>, IBuffer*>;
    
    public:
        MappedData() = default;
        MappedData(IDeviceContext* context,
                   IBuffer* buffer,
                   MapType mapType,
                   Uint mapFlags)
            : MappedData()
        {
            Map(context, buffer, mapType, mapFlags);
        }

        MappedData(MappedData& rhs)
            : mContext(std::move(rhs.mContext))
            , mBuffer(std::move(rhs.mBuffer))
            , mData(std::move(rhs.mData))
            , mMapType(std::move(rhs.mMapType))
            , mMapFlags(std::move(rhs.mMapFlags))
        {}

        MappedData& operator=(MappedData&& rhs) 
        {
            mContext = std::move(rhs.mContext);
            mBuffer = std::move(rhs.mBuffer);
            mData = std::move(rhs.mData);
            mMapType = std::move(rhs.mMapType);
            mMapFlags = std::move(rhs.mMapFlags);
            rhs.mContext = nullptr;
            rhs.mBuffer = nullptr;
            rhs.mData = nullptr;
            rhs.mMapType = MapType::Read;
            rhs.mMapFlags = 0;
            return *this;
        }

        ~MappedData() { UnMap(); }

        void Map(IDeviceContext* context,
                 IBuffer* buffer,
                 MapType mapType,
                 Uint mapFlags)
        {
            UnMap();
            mData = (T*)context->MapBuffer(buffer, mapType, mapFlags);
            if (mData != nullptr) {
                if constexpr (_StrongRef_) {
                    mContext = context->shared_from_this();
                    mBuffer = buffer->shared_from_this();
                }
                else {
                    mContext = context;
                    mBuffer = buffer;
                }
                mMapType = mapType;
                mMapFlags = mapFlags;
            }
        }

        void UnMap()
        {
            if (mBuffer) {
                mContext->UnMapBuffer(mBuffer, mMapType);
                mBuffer = nullptr;
                mMapType = MapType::Read;
                mMapFlags = 0;
            }
            mContext = nullptr;
            mData = nullptr;
        }

        operator T* () { return mData; }
        operator const T* () const { return mData; }
        T* operator->() { return mData; }
        const T* operator->() const { return mData; }

    private:
        ContextPtr mContext = nullptr;
        BufferPtr mBuffer = nullptr;
        T* mData = nullptr;
        MapType mMapType = MapType::Read;
        Uint mMapFlags = 0;
    };
}
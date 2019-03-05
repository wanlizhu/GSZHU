#pragma once

#include <NTGS/Graphics/IDeviceObject.h>

namespace NTGS {
    class IBufferView;

    enum BUFFER_MODE : UINT8 {
        BUFFER_MODE_UNDEFINED = 0,
        BUFFER_MODE_FORMATTED,
        BUFFER_MODE_STRUCTURED,
        BUFFER_MODE_RAW,
        BUFFER_MODE_NUM_MODES
    };

    struct BufferDesc : IDeviceObjectAttribs {
        UINT SizeInBytes = 0; // For a uniform buffer, this must be multiple of 16.
        BIND_FLAGS BindFlags = BIND_NONE;
        USAGE Usage = USAGE_DEFAULT;
        CPU_ACCESS_FLAGS CPUAccessFlags = CPU_ACCESS_NONE;
        BUFFER_MODE Mode = BUFFER_MODE_UNDEFINED;
        UINT ElementByteStride = 0;
        UINT64 CommandQueueMask = 1;

        BufferDesc() = default;
        explicit BufferDesc(UINT _SizeInBytes,
                            BIND_FLAGS _BindFlags,
                            USAGE _Usage = BufferDesc{}.Usage,
                            CPU_ACCESS_FLAGS _CPUAccessFlags = BufferDesc{}.CPUAccessFlags,
                            BUFFER_MODE _Mode = BufferDesc{}.Mode,
                            UINT _ElementByteStride = BufferDesc{}.ElementByteStride,
                            UINT64 _CommandQueueMask = BufferDesc{}.CommandQueueMask) noexcept 
            : SizeInBytes(_SizeInBytes)
            , BindFlags(_BindFlags)
            , Usage(_Usage)
            , Mode(_Mode)
            , ElementByteStride(_ElementByteStride)
            , CommandQueueMask(_CommandQueueMask) 
        {}

        bool operator == (const BufferDesc& RHS) const {
            return SizeInBytes == RHS.SizeInBytes &&
                BindFlags == RHS.BindFlags &&
                Usage == RHS.Usage &&
                CPUAccessFlags == RHS.CPUAccessFlags &&
                Mode == RHS.Mode &&
                ElementByteStride == RHS.ElementByteStride &&
                CommandQueueMask == RHS.CommandQueueMask;
        }
    };

    struct BufferData {
        const void* Data = nullptr;
        UINT DataSize = 0;

        BufferData() = default;
        BufferData(const void* _Data, UINT _DataSize) 
            : Data(_Data), DataSize(_DataSize) {}
    };

    class IBuffer : public IDeviceObject
    {
    public:
        virtual const BufferDesc& GetDesc()const = 0;
        virtual Weak<IBufferView> CreateView(const struct BufferViewDesc& ViewDesc) = 0;
        virtual Weak<IBufferView> GetDefaultView(BUFFER_VIEW_TYPE ViewType) = 0;
        virtual void* GetNativeHandle() = 0;
        virtual void SetState(RESOURCE_STATE State) = 0;
        virtual RESOURCE_STATE GetState() const = 0;
    };
}
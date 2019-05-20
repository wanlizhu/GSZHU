#include <GSZHU/STextureSubresourceData.h>

namespace GSZHU {
    STextureSubresourceData::STextureSubresourceData() noexcept 
    {}

    STextureSubresourceData::STextureSubresourceData(void *_Data, uint32_t _Stride, uint32_t _DepthStride) noexcept 
        : CPUData(_Data)
        , DeviceBuffer(nullptr)
        , DeviceBufferOffset(0)
        , Stride(_Stride)
        , DepthStride(_DepthStride)
    {}

    STextureSubresourceData::STextureSubresourceData(IBuffer *_DeviceBuffer, uint32_t _DeviceBufferOffset, uint32_t _Stride, uint32_t _DepthStride) noexcept 
        : CPUData(nullptr)
        , DeviceBuffer(_DeviceBuffer)
        , DeviceBufferOffset(_DeviceBufferOffset)
        , Stride(_Stride)
        , DepthStride(_DepthStride)
    {}
}
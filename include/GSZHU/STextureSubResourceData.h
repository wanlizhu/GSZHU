#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    struct STextureSubresourceData {
        const void* CPUData = nullptr; // Pointer to the subresource data in CPU memory.
        class IBuffer* DeviceBuffer = nullptr; // Pointer to the GPU buffer that contains subresource data.
        UINT DeviceBufferOffset = 0;
        UINT Stride = 0;
        UINT DepthStride = 0;

        STextureSubresourceData() noexcept {}
        STextureSubresourceData(void *_Data, UINT _Stride, UINT _DepthStride = 0) noexcept :
            CPUData(_Data),
            DeviceBuffer(nullptr),
            DeviceBufferOffset(0),
            Stride(_Stride),
            DepthStride(_DepthStride)
        {}
        STextureSubresourceData(IBuffer *_DeviceBuffer, UINT _DeviceBufferOffset, UINT _Stride, UINT _DepthStride = 0) noexcept :
            CPUData(nullptr),
            DeviceBuffer(_DeviceBuffer),
            DeviceBufferOffset(_DeviceBufferOffset),
            Stride(_Stride),
            DepthStride(_DepthStride)
        {}
    };
}
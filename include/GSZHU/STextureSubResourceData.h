#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    struct STextureSubresourceData {
        const void* CPUData = nullptr; // Pointer to the subresource data in CPU memory.
        class IBuffer* DeviceBuffer = nullptr; // Pointer to the GPU buffer that contains subresource data.
        uint32_t DeviceBufferOffset = 0;
        uint32_t Stride = 0;
        uint32_t DepthStride = 0;

        STextureSubresourceData() noexcept {}
        STextureSubresourceData(void *_Data, uint32_t _Stride, uint32_t _DepthStride = 0) noexcept :
            CPUData(_Data),
            DeviceBuffer(nullptr),
            DeviceBufferOffset(0),
            Stride(_Stride),
            DepthStride(_DepthStride)
        {}
        STextureSubresourceData(IBuffer *_DeviceBuffer, uint32_t _DeviceBufferOffset, uint32_t _Stride, uint32_t _DepthStride = 0) noexcept :
            CPUData(nullptr),
            DeviceBuffer(_DeviceBuffer),
            DeviceBufferOffset(_DeviceBufferOffset),
            Stride(_Stride),
            DepthStride(_DepthStride)
        {}
    };
}
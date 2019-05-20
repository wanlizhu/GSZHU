#pragma once

#include <GSZHU/SDeviceObjectAttribs.h>
#include <GSZHU/ENUM/EBindFlags.h>
#include <GSZHU/ENUM/EBufferMode.h>
#include <GSZHU/ENUM/EUsage.h>
#include <GSZHU/ENUM/ECPUAccessFlags.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct GSZHU_API SBufferDesc : public SDeviceObjectAttribs {
        uint32_t SizeInBytes = 0; // For a uniform buffer, this must be multiple of 16.
        EBIND_FLAGS BindFlags = BIND_NONE;
        EUSAGE Usage = USAGE_DEFAULT;
        ECPU_ACCESS_FLAGS CPUAccessFlags = CPU_ACCESS_NONE;
        EBUFFER_MODE Mode = BUFFER_MODE_UNDEFINED;
        uint32_t ElementByteStride = 0;
        uint64_t CommandQueueMask = 1;

        SBufferDesc() noexcept;
        SBufferDesc(uint32_t _SizeInBytes, EBIND_FLAGS _BindFlags,
            EUSAGE _Usage = SBufferDesc{}.Usage,
            ECPU_ACCESS_FLAGS _CPUAccessFlags = SBufferDesc{}.CPUAccessFlags,
            EBUFFER_MODE _Mode = SBufferDesc{}.Mode,
            uint32_t _ElementByteStride = SBufferDesc{}.ElementByteStride,
            uint64_t _CommandQueueMask = SBufferDesc{}.CommandQueueMask) noexcept;

        bool operator==(const SBufferDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SBufferDesc> {
        size_t operator()(const GSZHU::SBufferDesc& Desc) const {
            return GSZHU::ComputeHash(Desc.SizeInBytes,
                                      static_cast<int>(Desc.BindFlags),
                                      static_cast<int>(Desc.Usage),
                                      static_cast<int>(Desc.CPUAccessFlags),
                                      static_cast<int>(Desc.Mode),
                                      Desc.ElementByteStride,
                                      Desc.CommandQueueMask);
        }
    };
}
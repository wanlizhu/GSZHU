#pragma once

#include <GSZHU/SDeviceObjectAttribs.h>
#include <GSZHU/ENUM/EBindFlags.h>
#include <GSZHU/ENUM/EBufferMode.h>
#include <GSZHU/ENUM/EUsage.h>
#include <GSZHU/ENUM/ECPUAccessFlags.h>

namespace GSZHU {
    struct SBufferDesc : public SDeviceObjectAttribs {
        UINT SizeInBytes = 0; // For a uniform buffer, this must be multiple of 16.
        EBIND_FLAGS BindFlags = BIND_NONE;
        EUSAGE Usage = USAGE_DEFAULT;
        ECPU_ACCESS_FLAGS CPUAccessFlags = CPU_ACCESS_NONE;
        EBUFFER_MODE Mode = BUFFER_MODE_UNDEFINED;
        UINT ElementByteStride = 0;
        UINT64 CommandQueueMask = 1;

        SBufferDesc() noexcept {}
        SBufferDesc(UINT _SizeInBytes, EBIND_FLAGS _BindFlags) noexcept
            : SizeInBytes(_SizeInBytes)
            , BindFlags(_BindFlags)
        {}

        bool operator==(const SBufferDesc& rhs) const {
            return SizeInBytes == rhs.SizeInBytes
                && BindFlags == rhs.BindFlags
                && Usage == rhs.Usage
                && CPUAccessFlags == rhs.CPUAccessFlags
                && Mode == rhs.Mode
                && ElementByteStride == rhs.ElementByteStride
                && CommandQueueMask == rhs.CommandQueueMask;
        }
    };
}
#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // Allowed CPU access mode flags when mapping a resource
    // The enumeration is used by BufferDesc and TextureDesc 
    // Only USAGE_DYNAMIC resources can be mapped
    enum ECPU_ACCESS_FLAGS : uint8_t {
        CPU_ACCESS_NONE = 0x00, ///< No CPU access
        CPU_ACCESS_READ = 0x01, ///< A resource can be mapped for reading
        CPU_ACCESS_WRITE = 0x02  ///< A resource can be mapped for writing
    };
    DEFINE_FLAG_ENUM_OPS(ECPU_ACCESS_FLAGS)
}
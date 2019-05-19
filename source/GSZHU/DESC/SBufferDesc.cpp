#include <GSZHU/DESC/SBufferDesc.h>

namespace GSZHU {
    SBufferDesc::SBufferDesc() noexcept
    {}
        
    SBufferDesc::SBufferDesc(uint32_t _SizeInBytes,
                             EBIND_FLAGS _BindFlags,
                             EUSAGE _Usage,
                             ECPU_ACCESS_FLAGS _CPUAccessFlags,
                             EBUFFER_MODE _Mode,
                             uint32_t _ElementByteStride,
                             uint64_t _CommandQueueMask) noexcept
        : SizeInBytes(_SizeInBytes)
        , BindFlags(_BindFlags)
        , Usage(_Usage)
        , CPUAccessFlags(_CPUAccessFlags)
        , Mode(_Mode)
        , ElementByteStride(_ElementByteStride)
        , CommandQueueMask(_CommandQueueMask)
    {}

    bool SBufferDesc::operator==(const SBufferDesc& rhs) const {
        return SizeInBytes == rhs.SizeInBytes
            && BindFlags == rhs.BindFlags
            && Usage == rhs.Usage
            && CPUAccessFlags == rhs.CPUAccessFlags
            && Mode == rhs.Mode
            && ElementByteStride == rhs.ElementByteStride
            && CommandQueueMask == rhs.CommandQueueMask;
    }
}
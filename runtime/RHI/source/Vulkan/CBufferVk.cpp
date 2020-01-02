#include "CBufferVk.h"

namespace Wanlix
{

    VkBuffer CBufferVk::GetVkBuffer() const
    {
        auto s = shared_from_this();
    }

    void CBufferVk::SetAccessFlags(VkAccessFlags flags)
    {

    }

    VkAccessFlags CBufferVk::GetAccessFlags() const
    {
        return 0;
    }
}
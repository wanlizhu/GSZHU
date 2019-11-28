#include "Wanlix/Core3D/Graphics/RHI/ResourceDescriptor.h"

namespace Wanlix
{
    bool ResourceDescriptor::operator==(const ResourceDescriptor& rhs) const
    {
        return type == rhs.type
            && bindFlags == rhs.bindFlags
            && cpuAccess == rhs.cpuAccess
            && misc == rhs.misc;
    }

    bool ResourceDescriptor::operator!=(const ResourceDescriptor& rhs) const
    {
        return !(*this == rhs);
    }
}
#pragma once

#include <NTGS/Graphics/IDeviceObjectAttribs.h>
#include <NTGS/Core/Utilities.h>

namespace NTGS {
    class IObject : public std::enable_shared_from_this<IObject>, public NonCopyable {
    public:
        virtual SIZE GetID() const = 0;
        virtual SIZE TypeID() const = 0;
        virtual const char* TypeName() const = 0;
    };
}
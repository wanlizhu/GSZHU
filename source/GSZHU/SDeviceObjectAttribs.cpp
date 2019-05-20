#include <GSZHU/SDeviceObjectAttribs.h>

namespace GSZHU {
    SDeviceObjectAttribs::SDeviceObjectAttribs() noexcept 
    {}

    SDeviceObjectAttribs::SDeviceObjectAttribs(const char* _Name) noexcept 
        : Name(_Name) 
    {}
}
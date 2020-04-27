#include "render_device.h"

namespace djv
{
    DJV_API IRenderDevice* create_render_device()
    {
        return nullptr;
    }

    DJV_API void delete_render_device(IRenderDevice* device)
    {
        if (device)
        {
            delete device;
        }
    }
}
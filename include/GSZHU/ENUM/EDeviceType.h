#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum class EDeviceType : int {
        UNDEFINED = 0,  
        D3D11,          
        D3D12,          
        OpenGL,         
        OpenGLES,       
        Vulkan,         
        Metal           
    };
}
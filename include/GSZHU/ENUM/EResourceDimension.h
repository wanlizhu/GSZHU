#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // This enumeration is used by TextureDesc and TextureViewDesc 
    enum ERESOURCE_DIMENSION : UINT8 {
        RESOURCE_DIM_UNDEFINED = 0, 
        RESOURCE_DIM_BUFFER,        
        RESOURCE_DIM_TEX_1D,        
        RESOURCE_DIM_TEX_1D_ARRAY,  
        RESOURCE_DIM_TEX_2D,        
        RESOURCE_DIM_TEX_2D_ARRAY,  
        RESOURCE_DIM_TEX_3D,        
        RESOURCE_DIM_TEX_CUBE,      
        RESOURCE_DIM_TEX_CUBE_ARRAY,
        RESOURCE_DIM_NUM_DIMENSIONS 
    };
}
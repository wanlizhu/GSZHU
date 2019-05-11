#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // This enumeration defines filter type. It is used by SamplerDesc structure to define min, mag and mip filters.
    enum FILTER_TYPE : UINT8 {
        FILTER_TYPE_UNKNOWN = 0,           
        FILTER_TYPE_POINT,                 
        FILTER_TYPE_LINEAR,                
        FILTER_TYPE_ANISOTROPIC,           
        FILTER_TYPE_COMPARISON_POINT,      
        FILTER_TYPE_COMPARISON_LINEAR,     
        FILTER_TYPE_COMPARISON_ANISOTROPIC,
        FILTER_TYPE_MINIMUM_POINT,         
        FILTER_TYPE_MINIMUM_LINEAR,        
        FILTER_TYPE_MINIMUM_ANISOTROPIC,   
        FILTER_TYPE_MAXIMUM_POINT,         
        FILTER_TYPE_MAXIMUM_LINEAR,        
        FILTER_TYPE_MAXIMUM_ANISOTROPIC,   
        FILTER_TYPE_NUM_FILTERS            
    };
}
#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // It is used by BufferDesc and DrawAttribs
    enum EVALUE_TYPE : uint8_t {
        VT_UNDEFINED = 0, 
        VT_INT8,          
        VT_INT16,         
        VT_INT32,         
        VT_UINT8,         
        VT_UINT16,        
        VT_UINT32,        
        VT_FLOAT16,       
        VT_FLOAT32,       

        VT_NUM_TYPES      
    };
}
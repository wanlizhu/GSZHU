#pragma once

#include "GE2/BasicTypes.h"
#include "GE2/Utilities.h"
#include "ForwardDeclarations.h"

namespace GE2::RHI
{
    enum class EComparison
    {
        Disabled,       
        Never,          
        Always,         
        Less,           
        Equal,          
        NotEqual,       
        LessEqual,     
        Greater,        
        GreaterEqual,   
    };

    enum class EStencilOp
    {
        Keep,               
        Zero,               
        Replace,            
        Increase,           
        IncreaseSaturate,   
        Decrease,           
        DecreaseSaturate,   
        Invert              
    };

    struct GE2_IMPEXP DepthStencilState 
    {
        struct StencilDesc
        {
            EComparison compareOp = EComparison::Always;
            uint32_t reference = 0;
            uint32_t compareMask = 0;
            uint32_t writeMask = 0;
            EStencilOp depthFailOp = EStencilOp::Keep;
            EStencilOp stencilFailOp = EStencilOp::Keep;
            EStencilOp depthStencilPassOp = EStencilOp::Keep;
        };

        bool        depthTestEnable = true;
        bool        depthWriteEnable = true;
        EComparison depthCompareOp = EComparison::LessEqual;

        bool        depthBoundsTestEnable = false;
        float       depthBoundsMin = 0.f;
        float       depthBoundsMax = 0.f;

        bool        stencilTestEnable = false;
        StencilDesc front;
        StencilDesc back;
    };
}
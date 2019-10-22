#pragma once

#include "GE2/BasicTypes.h"
#include "GE2/Utilities.h"
#include "ForwardDeclarations.h"

namespace GE2::RHI
{
    enum class EBlendOp
    {
        Add,                
        Subtract,           
        ReverseSubtract,    
        Min,                
        Max
    };
    DEFINE_ENUM_OPS(EBlendOp)

    enum class EBlendFactor
    {
        Zero,                   
        One,                    
        SrcColor,               
        OneMinusSrcColor,       
        DstColor,               
        OneMinusDstColor,       
        SrcAlpha,               
        OneMinusSrcAlpha,       
        DstAlpha,               
        OneMinusDstAlpha,       
        BlendFactor,            
        OneMinusBlendFactor,    
        SrcAlphaSaturate,       
        Src1Color,              
        OneMinusSrc1Color,      
        Src1Alpha,              
        OneMinusSrc1Alpha       
    };
    DEFINE_ENUM_OPS(EBlendFactor)

    enum class ELogicOp
    {
        NoOp,
        Clear,
        And,
        AndReverse,
        Copy,
        AndInverted,
        XOR,
        Or,
        NotOr,
        Equivalent,
        Invert,
        OrReverse,
        CopyInverted,
        OrInverted,
        NotAnd,
        Set,
    };
    DEFINE_ENUM_OPS(ELogicOp)

    struct GE2_IMPEXP ColorWriteMask
    {
        bool writeRed   = true;
        bool writeGreen = true;
        bool writeBlue  = true;
        bool writeAlpha = true;
    };

    struct GE2_IMPEXP ColorBlendState
    {
        struct AttachmentState
        {
            bool           blendEnable = false;
            ColorWriteMask colorWriteMask;

            EBlendOp alphaBlendOp = EBlendOp::Add;
            EBlendOp colorBlendOp = EBlendOp::Add;

            EBlendFactor srcColorBlendFactor = EBlendFactor::One;
            EBlendFactor dstColorBlendFactor = EBlendFactor::Zero;

            EBlendFactor srcAlphaBlendFactor = EBlendFactor::One;
            EBlendFactor dstAlphaBlendFactor = EBlendFactor::Zero;
        };

        std::vector<AttachmentState> attachmentStateList;
        bool                         logicOpEnable = false;
        ELogicOp                     logicOp = ELogicOp::NoOp;
        std::array<float, 4>         blendConstants = { 1.f };
    };
}
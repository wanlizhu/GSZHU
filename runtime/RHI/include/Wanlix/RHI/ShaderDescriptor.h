#pragma once

#include "VertexShaderAttributes.h"
#include "FragmentShaderAttributes.h"
#include "VertexFormat.h"

namespace Wanlix
{
    struct ShaderMacro
    {
        const char* name = nullptr;
        const char* definition = nullptr;

        ShaderMacro() = default;
        ShaderMacro(const char* name, const char* definition = nullptr)
            : name(name)
            , definition(definition)
        {}
    };

    enum class ShaderType
    {
        Undefined,      
        Vertex,         
        TessControl,    
        TessEvaluation, 
        Geometry,       
        Fragment,       
        Compute,        
    };

    enum class ShaderSourceType
    {
        CodeString,     
        CodeFile,       
        BinaryBuffer,   
        BinaryFile,     
    };

    struct ShaderCompileFlags
    {
        enum
        {
            Debug = (1 << 0),     //!< Insert debug information.
            O1 = (1 << 1),        //!< Optimization level 1.
            O2 = (1 << 2),        //!< Optimization level 2.
            O3 = (1 << 3),        //!< Optimization level 3.
            WarnError = (1 << 4), //!< Warnings are treated as errors.
        };
    };

    struct ShaderStageFlags
    {
        enum
        {
            VertexStage = (1 << 0),
            TessControlStage = (1 << 1),
            TessEvaluationStage = (1 << 2),
            GeometryStage = (1 << 3),
            FragmentStage = (1 << 4),
            ComputeStage = (1 << 5),
            AllTessStages = (TessControlStage | TessEvaluationStage),
            AllGraphicsStages = (VertexStage | AllTessStages | GeometryStage | FragmentStage),
            AllStages = (AllGraphicsStages | ComputeStage),
        };
    };
}
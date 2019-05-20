#pragma once

#include <GSZHU/SShaderMacro.h>
#include <GSZHU/DESC/SShaderDesc.h>
#include <GSZHU/ENUM/EShaderSourceLanguage.h>

namespace GSZHU {
    struct GSZHU_API SShaderCreationAttribs {
        union {
            const char* FilePath;
            const char* SourceCode;
            struct {
                const void* ByteCode = nullptr;
                size_t ByteCodeSize = 0;
            } Binary;
        };

        const char* EntryPoint = "main";
        const SShaderMacro* Macros = nullptr;

        bool UseCombinedTextureSamplers = false;
        const char* CombinedSamplerSuffix = "_sampler";

        SShaderDesc Desc;
        ESHADER_SOURCE_LANGUAGE SourceLanguage = SHADER_SOURCE_LANGUAGE_DEFAULT;
    };

}
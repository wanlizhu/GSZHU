#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIShaderReflectionVk;

    /*
     * It's possible to hard-code the shader reflection by derivating this class and overriding 'Build()' method
    */
    class GIShaderReflectionBuilderVk
    {
    public:
        GIShaderReflectionBuilderVk();
        GIShaderReflectionBuilderVk& LoadFromFile(const std::filesystem::path& path);
        SharedPtr<GIShaderReflectionVk> Build();

    protected:
        SharedPtr<GIShaderReflectionVk> mShaderReflection;
    };
}
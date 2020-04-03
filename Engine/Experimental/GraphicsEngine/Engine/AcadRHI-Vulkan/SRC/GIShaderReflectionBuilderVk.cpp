#include "GIShaderReflectionBuilderVk.h"
#include "GIShaderReflectionVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIShaderReflectionBuilderVk::GIShaderReflectionBuilderVk()
        : mShaderReflection(new GIShaderReflectionVk())
    {}

    GIShaderReflectionBuilderVk& GIShaderReflectionBuilderVk::LoadFromFile(const std::filesystem::path& path)
    {
        return *this;
    }

    SharedPtr<GIShaderReflectionVk> GIShaderReflectionBuilderVk::Build()
    {
        return mShaderReflection;
    }
}
#pragma once

#include "RHI/IDeviceObject.h"
#include "RHI/IShaderVariable.h"
#include "RHI/ISampler.h"
#include "RHI/IResourceTable.h"

namespace Wanlix
{
    struct ShaderDesc : public DeviceObjectDesc
    {
        EShaderType shaderType = EShaderType::Vertex;
        EShaderProfile shaderProfile = EShaderProfile::Default;
        EShaderVariableType defaultVariableType = EShaderVariableType::Static;
        Array<ShaderVariableDesc> variables;
        Array<StaticSampler> staticSamplers;
    };

    struct ShaderMacro
    {
        String name;
        String value;
    };

    struct ShaderCreateInfo
    {
        EShaderSource sourceType = EShaderSource::None;
        String sourceFile;
        String sourceCode;
        ByteArray byteCode;

        String entryPoint = "main";
        Array<ShaderMacro> macros;
        Optional<String> samplerSuffix = "_sampler";
        EShaderLanguage language = EShaderLanguage::Default;
        ShaderDesc desc;
    };

    class IShader : public IDeviceObject
    {
    public:
        virtual Bool IsCompiled() const = 0;
        virtual void BindResources(IResourceTable* table, EBindResourceFlags flags) = 0;
        virtual Uint GetVariableCount() const = 0;
        virtual IShaderVariable* GetVariable(const String& name) = 0;
        virtual IShaderVariable* GetVariable(Uint index) = 0;
    };
}
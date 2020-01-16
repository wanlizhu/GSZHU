#pragma once

#include "Graphics/RHI/IDeviceObject.h"
#include "Graphics/RHI/IShaderVariable.h"
#include "Graphics/RHI/ISampler.h"
#include "Graphics/RHI/IResourceTable.h"

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
        virtual EShaderType GetShaderType() const = 0;
        virtual Bool Compile(const ShaderCreateInfo& info) = 0;
        virtual Bool IsCompiled() const = 0;
        virtual void BindResources(IResourceTable* table) = 0;
        virtual Uint GetVariableCount() const = 0;
        virtual IShaderVariable* GetVariable(const String& name) = 0;
        virtual IShaderVariable* GetVariable(Uint index) = 0;

        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const ShaderDesc& GetDesc() const = 0;
    };
}
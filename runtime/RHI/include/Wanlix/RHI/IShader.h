#pragma once

#include "IDeviceObject.h"

namespace Wanlix
{
    struct ShaderMacro
    {
        String name;
        String definition;
    };

    struct ShaderDesc : public DeviceObjectDesc
    {
        ShaderType shaderType = ShaderType::Unknown;
        ShaderProfile profile = ShaderProfile::Default;
    };

    struct ShaderCreateInfo
    {
        Wstring filePath;
        String source;
        ByteArray byteArray;

        String entryPoint = "main";
        Array<ShaderMacro> macros;
        Bool useCombinedSampler = false;
        String combinedSamplerSuffix = "_sampler";

        ShaderDesc desc;
        ShaderLanguage language = ShaderLanguage::Default;
        Pvoid compilerOutput = nullptr;
    };

    struct ShaderResourceDesc
    {
        String name;
        ShaderResourceType type = ShaderResourceType::Unknown;
        Uint arraySize = 0;
    };

    class RHI_API IShader : public IDeviceObject
    {
        CLASSINFO(IShader)
        INHERIT_SHARED_FROM_THIS(IDeviceObject)
    public:
        virtual ~IShader() = default;

        /* interfaces inherited from IDeviceObject */
        virtual Pvoid GetNativeHandle() const = 0;
        virtual ShaderDesc const& GetDesc() const override = 0;
        virtual IDevice* GetDevice() const override = 0;

        virtual Uint GetNumResources() const = 0;
        virtual ShaderResourceDesc GetResource(Uint index) const = 0;
    };
}
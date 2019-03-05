#pragma once

#include <NTGS/Graphics/IDeviceObject.h>
#include <NTGS/Graphics/IResourceMapping.h>
#include <NTGS/Graphics/ISampler.h>
#include <fstream>
#include <vector>

namespace NTGS {
    enum SHADER_TYPE : UINT {
        SHADER_TYPE_UNKNOWN = 0x000, 
        SHADER_TYPE_VERTEX = 0x001, 
        SHADER_TYPE_PIXEL = 0x002, 
        SHADER_TYPE_GEOMETRY = 0x004,
        SHADER_TYPE_HULL = 0x008, 
        SHADER_TYPE_DOMAIN = 0x010, 
        SHADER_TYPE_COMPUTE = 0x020  
    };

    enum SHADER_PROFILE : UINT8 {
        SHADER_PROFILE_DEFAULT = 0,
        SHADER_PROFILE_DX_4_0,
        SHADER_PROFILE_DX_5_0,
        SHADER_PROFILE_DX_5_1,
        SHADER_PROFILE_GL_4_2
    };

    enum SHADER_SOURCE_LANGUAGE : UINT {
        SHADER_SOURCE_LANGUAGE_DEFAULT = 0,
        SHADER_SOURCE_LANGUAGE_HLSL,
        SHADER_SOURCE_LANGUAGE_GLSL
    };

    enum SHADER_VARIABLE_TYPE : UINT8 {
        SHADER_VARIABLE_TYPE_STATIC = 0,
        SHADER_VARIABLE_TYPE_MUTABLE,
        SHADER_VARIABLE_TYPE_DYNAMIC,
        SHADER_VARIABLE_TYPE_NUM_TYPES
    };

    static_assert(SHADER_VARIABLE_TYPE_STATIC == 0 &&
                  SHADER_VARIABLE_TYPE_MUTABLE == 1 && 
                  SHADER_VARIABLE_TYPE_DYNAMIC == 2 &&
                  SHADER_VARIABLE_TYPE_NUM_TYPES == 3,
                  "SHADER_VARIABLE_TYPE_* values MUST be 0,1,2");

    enum BIND_SHADER_RESOURCES_FLAGS : UINT {
        BIND_SHADER_RESOURCES_UPDATE_STATIC = (0x01 << SHADER_VARIABLE_TYPE_STATIC),
        BIND_SHADER_RESOURCES_UPDATE_MUTABLE = (0x01 << SHADER_VARIABLE_TYPE_MUTABLE),
        BIND_SHADER_RESOURCES_UPDATE_DYNAMIC = (0x01 << SHADER_VARIABLE_TYPE_DYNAMIC),
        BIND_SHADER_RESOURCES_UPDATE_ALL = (BIND_SHADER_RESOURCES_UPDATE_STATIC | BIND_SHADER_RESOURCES_UPDATE_MUTABLE | BIND_SHADER_RESOURCES_UPDATE_DYNAMIC),
        BIND_SHADER_RESOURCES_KEEP_EXISTING = 0x08,
        BIND_SHADER_RESOURCES_VERIFY_ALL_RESOLVED = 0x10
    };

    struct ShaderVariableDesc {
        const char* Name = nullptr;
        SHADER_VARIABLE_TYPE Type = SHADER_VARIABLE_TYPE_STATIC;

        ShaderVariableDesc() = default;
        ShaderVariableDesc(const char* _Name, SHADER_VARIABLE_TYPE _Type) 
            : Name(_Name), Type(_Type) {}
    };

    struct StaticSamplerDesc {
        const char* SamplerOrTextureName = nullptr;
        SamplerDesc Desc;

        StaticSamplerDesc() = default;
        StaticSamplerDesc(const char* _SamplerOrTextureName, const SamplerDesc& _Desc) noexcept 
            : SamplerOrTextureName(_SamplerOrTextureName), Desc(_Desc) {}
    };

    struct ShaderDesc : IDeviceObjectAttribs {
        SHADER_TYPE ShaderType = SHADER_TYPE_VERTEX;
        bool bCacheCompiledShader = false;
        SHADER_PROFILE TargetProfile = SHADER_PROFILE_DEFAULT;
        SHADER_VARIABLE_TYPE DefaultVariableType = SHADER_VARIABLE_TYPE_STATIC;
        const ShaderVariableDesc* VariableDesc = nullptr;
        UINT NumVariables = 0;
        UINT NumStaticSamplers = 0;
        const StaticSamplerDesc* StaticSamplers = nullptr;
    };

    class IShaderSourceInputStreamFactory {
    public:
        virtual std::ifstream CreateInputStream(const char* Name) = 0;
    };

    struct ShaderMacro {
        const char* Name = nullptr;
        const char* Definition = nullptr;

        ShaderMacro() = default;
        ShaderMacro(const char* _Name, const char* _Def) noexcept 
            : Name(_Name), Definition(_Def) {}
    };

    struct ShaderCreationAttribs {
        const char* FilePath = nullptr;
        IShaderSourceInputStreamFactory* ShaderSourceStreamFactory = nullptr;
        class IHLSL2GLSLConversionStream** ConversionStream = nullptr;
        const char* Source = nullptr;
        const void* ByteCode = nullptr;
        size_t ByteCodeSize = 0;
        const char* EntryPoint = "main";
        const ShaderMacro* Macros = nullptr;
        bool UseCombinedTextureSamplers = false;
        const char* CombinedSamplerSuffix = "_sampler";
        ShaderDesc Desc;
        SHADER_SOURCE_LANGUAGE SourceLanguage = SHADER_SOURCE_LANGUAGE_DEFAULT;
        std::vector<char>* CompilerOutput = nullptr;
    };

    class IShaderVariable : public IObject {
    public:
        virtual void Set(IDeviceObject* Object) = 0;
        virtual void SetArray(IDeviceObject* const* Objects, UINT FirstElement, UINT NumElements) = 0;
        virtual SHADER_VARIABLE_TYPE GetType() const = 0;
        virtual UINT GetArraySize()const = 0;
        virtual const char* GetName()const = 0;
        virtual UINT GetIndex()const = 0;
    };

    class IShader : public IDeviceObject {
    public:
        virtual const ShaderDesc& GetDesc()const = 0;
        virtual void BindResources(IResourceMapping* ResourceMapping, UINT Flags) = 0;
        virtual IShaderVariable* GetShaderVariable(const char* Name) = 0;
        virtual UINT GetVariableCount() const = 0;
        virtual IShaderVariable* GetShaderVariable(UINT Index) = 0;
    };
}
#pragma once

#include "IDeviceObject.h"
#include "EResourceFormat.h"

namespace GE2::RHI
{
    enum class EResourceType : uint32_t
    {
        Unknown = 0,
        Buffer,
        Texture1D,
        Texture2D,
        Texture3D,
        TextureCube,
        Texture2DMS, // 2D multi - sampled texture
    };

    enum class EResourceState : uint32_t
    {
        Unknown = 0,
        Uninitialized,
        Common,
        VertexBuffer,
        ConstantBuffer,
        IndexBuffer,
        RenderTarget,
        UnorderedAccess,
        DepthStencil,
        ShaderResource,
        StreamOut,
        IndirectArgument,
        CopyDest,
        CopySource,
        ResolveDest,
        ResolveSource,
        Present,
        GenericRead,
        Predication,
        NonPixelShader,
        AccelerationStructure,
    };

    class GE2_IMPEXP IResource : public IDeviceObject
    {
    public:
        static const uint32_t kMaxPossible = (uint32_t)-1;

        IResource() = default;

        // IDeviceObject's abstract virtual methods
        virtual ~IResource() = 0;
        virtual bool   Initialize() = 0;
        virtual void   Destroy() = 0;
        virtual CSTR   GetName() const = 0;
        virtual HANDLE GetHandle() const = 0;

        virtual EResourceType      GetType()        const = 0;
        virtual EResourceBindFlags GetBindFlags()   const = 0;
        virtual bool               IsGlobalState()  const = 0;
        virtual EResourceState     GetGlobalState() const = 0;
        virtual EResourceState     GetSubresourceState(uint32_t slice, uint32_t mip) const = 0;
    };
}
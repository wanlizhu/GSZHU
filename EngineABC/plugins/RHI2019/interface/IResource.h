#pragma once

#include "IDeviceObject.h"
#include "IResourceView.h"
#include "EResourceFormat.h"

namespace ZHU
{
	enum class EResourceType
	{
		Buffer,       // Can be bound to all shader - stages
		Texture1D,    // Can be bound as render-target, shader-resource and UAV
		Texture2D,    // ditto
		Texture3D,    // ditto
		TextureCube,  // ditto
		Texture2DMS,  // (2D multi-sampled texture) ditto
	};

	enum class EResourceState
	{
		Undefined,
		PreInitialized,
		Common,
		VertexBuffer,
		ConstantBuffer,
		IndexBuffer,
		RenderTarget,
		UnorderedAccess,
		DepthStencil,
		ShaderResource,
		StreamOut,
		IndirectArg,
		CopyDest,
		CopySource,
		ResolveDest,
		ResolveSource,
		Present,
		GenericRead,
		Predication,
		NonPixelShader,
#ifdef RHI_USE_D3D12
		AccelerationStructure,
#endif
	};

	enum class EResourceBindFlags
	{
		None = 0x0,             
		Vertex = 0x1,           
		Index = 0x2,            
		Constant = 0x4,         
		StreamOutput = 0x8,     
		ShaderResource = 0x10,  
		UnorderedAccess = 0x20, 
		RenderTarget = 0x40,    
		DepthStencil = 0x80,    
		IndirectArg = 0x100,    
#ifdef RHI_USE_D3D12
		AccelerationStructure = 0x80000000, 
#endif
	};

	class IResource : public IDeviceObject
	{
	public:
		virtual ~IResource() = 0;
		virtual EResourceType GetType() const = 0;
		virtual EResourceBindFlags GetBindFlags() const = 0;
		virtual bool IsStateGlobal() const = 0;
		virtual EResourceState GetGlobalState() const = 0;
		virtual EResourceState GetSubresourceState(uint32_t slice, uint32_t mip) const = 0;
		
		virtual IResourceView* CreateView(const ResourceViewDesc& desc) = 0;
		virtual void InvalidateViews() const = 0;
	};
}
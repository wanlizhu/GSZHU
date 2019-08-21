#pragma once

#include <d3d12.h>
#include <d3dcompiler.h>
#include <comdef.h>
#include <dxgi1_4.h>
#include <dxgiformat.h>
#include <memory>
#include <string>
#include <vector>
#include "EngineConfig.h"
#include "RHI/Formats.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

#define DEFINE_SMART_COM_PTR(_a) _COM_SMARTPTR_TYPEDEF(_a, __uuidof(_a))

#if LOG_ENABLED
#define CHECK_HRESULT(hr) { if (FAILED(hr)) { D3DTranceHResult(hr, __FILE__, __LINE__); } }
#else
#define CHECK_HRESULT(hr) hr
#endif

#define UNSUPPORTED_IN_D3D(msg) LOG(ERROR) << msg << " is not supported in D3D.";
// The max scalars supported by our driver
#define MAX_PAYLOAD_SIZE_IN_BYTES (14 * sizeof(float))  

#define GET_COM_INTERFACE(base, type, var)\
	DEFINE_SMART_COM_PTR(type);\
	type##Ptr var;\
	{\
		HRESULT hr = base->QueryInterface(IID_PPV_ARGS(&var));\
		CHECK_HRESULT(hr);\
	}
	



namespace GS
{
	class DescriptorSet;

	DEFINE_SMART_COM_PTR(ID3D12StateObject);
	DEFINE_SMART_COM_PTR(ID3D12Device);
	DEFINE_SMART_COM_PTR(ID3D12GraphicsCommandList);
	DEFINE_SMART_COM_PTR(ID3D12Debug);
	DEFINE_SMART_COM_PTR(ID3D12CommandQueue);
	DEFINE_SMART_COM_PTR(ID3D12CommandAllocator);
	DEFINE_SMART_COM_PTR(ID3D12DescriptorHeap);
	DEFINE_SMART_COM_PTR(ID3D12Resource);
	DEFINE_SMART_COM_PTR(ID3D12Fence);
	DEFINE_SMART_COM_PTR(ID3D12PipelineState);
	DEFINE_SMART_COM_PTR(ID3D12RootSignature);
	DEFINE_SMART_COM_PTR(ID3D12QueryHeap);
	DEFINE_SMART_COM_PTR(ID3D12CommandSignature);
	DEFINE_SMART_COM_PTR(IUnknown);

	using ObjectHandle = IUnknownPtr;
	using HeapCPUHandle = D3D12_CPU_DESCRIPTOR_HANDLE;
	using HeapGPUHandle = D3D12_GPU_DESCRIPTOR_HANDLE;

	using WindowHandle = HWND;
	using SharedLibraryHandle = HMODULE;
	using DeviceHandle = ID3D12DevicePtr;
	using CommandListHandle = ID3D12GraphicsCommandListPtr;
	using CommandQueueHandle = ID3D12CommandQueuePtr;
	using CommandQueueType = D3D12_COMMAND_LIST_TYPE;
	using CommandAllocatorHandle = ID3D12CommandAllocatorPtr;
	using CommandSignatureHandle = ID3D12CommandSignaturePtr;
	using FenceHandle = ID3D12FencePtr;
	using ResourceHandle = ID3D12ResourcePtr;
	using RTVHandle = std::shared_ptr<DescriptorSet>;
	using DSVHandle = std::shared_ptr<DescriptorSet>;
	using SRVHandle = std::shared_ptr<DescriptorSet>;
	using SamplerHandle = std::shared_ptr<DescriptorSet>;
	using UAVHandle = std::shared_ptr<DescriptorSet>;
	using CBVHandle = std::shared_ptr<DescriptorSet>;
	using FBOHandle = void*;
	using GPUAddress = D3D12_GPU_VIRTUAL_ADDRESS;
	using QueryHeapHandle = ID3D12QueryHeapPtr;

	using GraphicsStateHandle = ID3D12PipelineStatePtr;
	using ComputeStateHandle = ID3D12PipelineStatePtr;
	using ShaderHandle = D3D12_SHADER_BYTECODE;
	using RootSignatureHandle = ID3D12RootSignaturePtr;
	using DescriptorHeapHandle = ID3D12DescriptorHeapPtr;

	using VAOHandle = void*;
	using VertexShaderHandle = void*;
	using FragmentShaderHandle = void*;
	using DomainShaderHandle = void*;
	using HullShaderHandle = void*;
	using GeometryShaderHandle = void*;
	using ComputeShaderHandle = void*;
	using ProgramHandle = void*;
	using DepthStencilStateHandle = void*;
	using RasterizerStateHandle = void*;
	using BlendStateHandle = void*;
	using DescriptorSetApiHandle = void*;









	D3D_FEATURE_LEVEL GetD3DFeatureLevel(uint32_t majorVersion, uint32_t minorVersion);
	void D3DTranceHResult(HRESULT hr, const char* file, uint32_t line);

	struct DXGIFormatDesc
	{
		EResourceFormat GSFormat;
		DXGI_FORMAT DXGIFormat;
	};

	extern const DXGIFormatDesc kDXGIFormatDescArray[];

	inline DXGI_FORMAT GetDXGIFormat(EResourceFormat format)
	{
		assert(kDXGIFormatDescArray[(uint32_t)format].GSFormat == format);
		return kDXGIFormatDescArray[(uint32_t)format].DXGIFormat;
	}

	inline DXGI_FORMAT GetTypelessDepthFormat(EResourceFormat format)
	{
		switch (format)
		{
		case EResourceFormat::D16Unorm:
			return DXGI_FORMAT_R16_TYPELESS;
		case EResourceFormat::D32FloatS8X24:
			return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		case EResourceFormat::D24UnormS8:
			return DXGI_FORMAT_R24G8_TYPELESS;
		case EResourceFormat::D32Float:
			return DXGI_FORMAT_R32_TYPELESS;
		default:
			assert(IsDepthFormat(format) == false);
			return kDXGIFormatDescArray[(uint32_t)format].DXGIFormat;
		}
	}

	inline std::string to_string(D3D_FEATURE_LEVEL featureLevel)
	{
		switch (featureLevel)
		{
		case D3D_FEATURE_LEVEL_9_1:
			return "D3D_FEATURE_LEVEL_9_1";
		case D3D_FEATURE_LEVEL_9_2:
			return "D3D_FEATURE_LEVEL_9_2";
		case D3D_FEATURE_LEVEL_9_3:
			return "D3D_FEATURE_LEVEL_9_3";
		case D3D_FEATURE_LEVEL_10_0:
			return "D3D_FEATURE_LEVEL_10_0";
		case D3D_FEATURE_LEVEL_10_1:
			return "D3D_FEATURE_LEVEL_10_1";
		case D3D_FEATURE_LEVEL_11_0:
			return "D3D_FEATURE_LEVEL_11_0";
		case D3D_FEATURE_LEVEL_11_1:
			return "D3D_FEATURE_LEVEL_11_1";
		case D3D_FEATURE_LEVEL_12_0:
			return "D3D_FEATURE_LEVEL_12_0";
		case D3D_FEATURE_LEVEL_12_1:
			return "D3D_FEATURE_LEVEL_12_1";
		default:
			assert(false); 
			return "";
		}
	}

	enum class ERTBuildFlags
	{
		None = 0,
		AllowUpdate = 0x1,
		AllowCompaction = 0x2,
		FastTrace = 0x4,
		FastBuild = 0x8,
		MinimizeMemory = 0x10,
		PerformUpdate = 0x20,
	};
	ENUM_CLASS_OPERATORS(ERTBuildFlags);

	inline std::string to_string(ERTBuildFlags flags)
	{
		switch (flags)
		{
		case ERTBuildFlags::None:
			return "None";
		case ERTBuildFlags::AllowUpdate:
			return "AllowUpdate";
		case ERTBuildFlags::AllowCompaction:
			return "AllowCompaction";
		case ERTBuildFlags::FastTrace:
			return "FastTrace";
		case ERTBuildFlags::FastBuild:
			return "FastBuild";
		case ERTBuildFlags::MinimizeMemory:
			return "MinimizeMemory";
		case ERTBuildFlags::PerformUpdate:
			return "PerformUpdate";
		default:
			assert(false);
			return "";
		}
	}

	template<typename BlobType>
	inline size_t GetBlobData(BlobType* blob, std::vector<unsigned char>* data)
	{
		data->clear();
		data->resize(blob->GetBufferSize());
		std::memcpy(data->data(), blob->GetBufferPointer(), blob->GetBufferSize());
		return data->size();
	}

	inline D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS 
		GetDXRBuildFlags(ERTBuildFlags buildFlags)
	{
		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS dxr = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;

		if (is_set(buildFlags, ERTBuildFlags::AllowUpdate))
			dxr |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
		if (is_set(buildFlags, ERTBuildFlags::AllowCompaction))
			dxr |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_COMPACTION;
		if (is_set(buildFlags, ERTBuildFlags::FastTrace))
			dxr |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
		if (is_set(buildFlags, ERTBuildFlags::FastBuild))
			dxr |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_BUILD;
		if (is_set(buildFlags, ERTBuildFlags::MinimizeMemory))
			dxr |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_MINIMIZE_MEMORY;
		if (is_set(buildFlags, ERTBuildFlags::PerformUpdate))
			dxr |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE;

		return dxr;
	}
}
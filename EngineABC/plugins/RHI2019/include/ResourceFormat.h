#pragma once

#include <stdint.h>

namespace ZHU
{
	enum class EResourceBindFlags : uint32_t
	{
		None = 0x0,             ///< The resource will not be bound the pipeline. Use this to create a staging resource
		Vertex = 0x1,           ///< The resource will be bound as a vertex-buffer
		Index = 0x2,            ///< The resource will be bound as a index-buffer
		Constant = 0x4,         ///< The resource will be bound as a constant-buffer
		StreamOutput = 0x8,     ///< The resource will be bound to the stream-output stage as an output buffer
		ShaderResource = 0x10,  ///< The resource will be bound as a shader-resource
		UnorderedAccess = 0x20, ///< The resource will be bound as an UAV
		RenderTarget = 0x40,    ///< The resource will be bound as a render-target
		DepthStencil = 0x80,    ///< The resource will be bound as a depth-stencil buffer
		IndirectArg = 0x100,    ///< The resource will be bound as an indirect argument buffer
#ifdef RHI_USE_D3D12
		AccelerationStructure = 0x80000000,  ///< The resource will be bound as an acceleration structure
#endif
	};

	enum class EResourceFormat : uint32_t
	{
		Unknown,
		R8Unorm,
		R8Snorm,
		R16Unorm,
		R16Snorm,
		RG8Unorm,
		RG8Snorm,
		RG16Unorm,
		RG16Snorm,
		RGB16Unorm,
		RGB16Snorm,
		R24UnormX8,
		RGB5A1Unorm,
		RGBA8Unorm,
		RGBA8Snorm,
		RGB10A2Unorm,
		RGB10A2Uint,
		RGBA16Unorm,
		RGBA8UnormSrgb,
		R16Float,
		RG16Float,
		RGB16Float,
		RGBA16Float,
		R32Float,
		R32FloatX32,
		RG32Float,
		RGB32Float,
		RGBA32Float,
		R11G11B10Float,
		RGB9E5Float,
		R8Int,
		R8Uint,
		R16Int,
		R16Uint,
		R32Int,
		R32Uint,
		RG8Int,
		RG8Uint,
		RG16Int,
		RG16Uint,
		RG32Int,
		RG32Uint,
		RGB16Int,
		RGB16Uint,
		RGB32Int,
		RGB32Uint,
		RGBA8Int,
		RGBA8Uint,
		RGBA16Int,
		RGBA16Uint,
		RGBA32Int,
		RGBA32Uint,

		BGRA8Unorm,
		BGRA8UnormSrgb,

		BGRX8Unorm,
		BGRX8UnormSrgb,
		Alpha8Unorm,
		Alpha32Float,
		R5G6B5Unorm,

		// Depth-stencil
		D32Float,
		D16Unorm,
		D32FloatS8X24,
		D24UnormS8,

		// Compressed formats
		BC1Unorm,   // DXT1
		BC1UnormSrgb,
		BC2Unorm,   // DXT3
		BC2UnormSrgb,
		BC3Unorm,   // DXT5
		BC3UnormSrgb,
		BC4Unorm,   // RGTC Unsigned Red
		BC4Snorm,   // RGTC Signed Red
		BC5Unorm,   // RGTC Unsigned RG
		BC5Snorm,   // RGTC Signed RG
		BC6HS16,
		BC6HU16,
		BC7Unorm,
		BC7UnormSrgb,

		Count
	};

	enum class FormatType
	{
		Unknown,        ///< Unknown format Type
		Float,          ///< Floating-point formats
		Unorm,          ///< Unsigned normalized formats
		UnormSrgb,      ///< Unsigned normalized SRGB formats
		Snorm,          ///< Signed normalized formats
		Uint,           ///< Unsigned integer formats
		Sint            ///< Signed integer formats
	};
}
#pragma once

#include "Graphics/RHI/Constants.h"

namespace ZHU
{
	struct ZHU_GS_API DeviceCaps
	{
		struct ZHU_GS_API NDCAttribs
		{
			const float MinZ;
			const float ZtoDepth; // NDC z to depth scale
			const float YtoV; // Scale to transform NDC y coordinate to texture V coordinate
		};

		EDevice DeviceType = EDevice::D3D12;
		int MajorVersion = 0;
		int MinorVersion = 0;
		bool SeparableProgram = true;
		bool IndirectRendering = true;
		bool WireframeFill = true;
		bool MultithreadedResourceCreation = true;
		bool ComputeShader = true;
		bool GeometryShader = true;
		bool Tessellation = true;

		// Sampler Caps
		bool BorderSamplingMode = true;
		bool AnisotropicFiltering = true;
		bool LODBias = true;

		// Texture Caps
		bool Texture1D = true;
		bool Texture1DArray = true;
		bool Texture2DMS = true;
		bool Texture2DMSArray = true;
		bool TextureView = true;
		bool CubemapArray = true;

		NDCAttribs GetNDCAttribs() const;
	};
}
#pragma once

#include "Graphics/RHI/Constants.h"
#include "Graphics/Tools.h"
#include "Graphics/RHI/IBuffer.h"
#include "Graphics/RHI/ITexture.h"

namespace ZHU
{
	struct ZHU_GS_API CopyTextureAttribs
	{
		ITexture::Pointer SourceTexture = nullptr;
		uint32_t SourceMipLevel = 0;
		uint32_t SourceSlice = 0;
		Box<int> SourceBox;

		ITexture::Pointer DestTexture = nullptr;
		uint32_t DestMipLevel = 0;
		uint32_t DestSlice = 0;
		uint32_t DestOffsets[3] = { 0 };
	};


	struct ZHU_GS_API DrawAttribs
	{
		union {
			uint32_t NumVertices = 0;
			uint32_t NumIndices;
		};
		bool IsIndexed = false;
		EComponent IndexType = EComponent::Undefined;
		bool VerifyStates = false;

		uint32_t NumInstances = 1;
		uint32_t BaseVertex = 0;
		union {
			uint32_t StartVertexLocation = 0;
			uint32_t FirstIndexLocation;
		};
		uint32_t FirstInstanceLocation = 0;

		// Indirect draw
		IBuffer::Pointer IndirectDrawAttribs = nullptr;
		uint32_t IndirectDrawOffset = 0;
	};


	struct ZHU_GS_API DispatchComputeAttribs
	{
		uint32_t GroupCountX = 1;
		uint32_t GroupCountY = 1;
		uint32_t GroupCountZ = 1;

		IBuffer::Pointer IndirectDispatchAttribs = nullptr;
		uint32_t IndirectDispatchOffset = 0;
	};


	struct ZHU_GS_API Viewport
	{
		float StartPoint[2] = { 0.f, 0.f }; // Top-left
		float Width = 0.f;
		float Height = 0.f;

		float MinDepth = 0.f;
		float MaxDepth = 1.f;
	};


	struct ZHU_GS_API Rect
	{
		union {
			struct {
				struct { int min; int max; } x;
				struct { int min; int max; } y;
			};
			struct {
				int left, right, bottom, top;
			};
			int data[4] = { 0 };
		};

		Rect() = default;
		Rect(int left, int right, int bottom, int top) noexcept
			: x{ left, right }
			, y{ bottom, top }
		{}

		inline std::array<int, 2> LTPoint() const { return { left, top }; }
		inline std::array<int, 2> RBPoint() const { return { right, bottom }; }
		inline operator bool() const { return operator!=(*this); }
		inline bool operator==(const Rect& other) const { return LTPoint() == RBPoint(); }
		inline bool operator!=(const Rect& other) const { return !operator==(other); }
	};
}
#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include "ICopyContext.h"
#include "IRenderPipelineState.h"

namespace ZHU
{
	class IBuffer;

	enum class EDrawType
	{
		Array,
		Indexed,
		ArrayInstanced,
		IndexedInstanced,
		ArrayIndirect,
		IndexedIndirect,
	};

	struct DrawAttribs
	{
		EDrawType mDrawType;
		union 
		{
			struct
			{
				uint32_t mVertexCount;
				uint32_t mStartVertexLocation;
			} mDrawArray;
			struct
			{
				uint32_t mVertexCount;
				uint32_t mInstanceCount;
				uint32_t mStartVertexLocation; // The location of the first vertex to read from the vertex buffers (offset in vertices)
				uint32_t mStartInstanceLocation; // A value which is added to each index before reading per-instance data from the vertex buffer
			} mDrawArrayInstanced;
			struct
			{
				uint32_t mIndexCount;
				uint32_t mStartIndexLocation; // The location of the first index to read from the index buffer (offset in indices)
				uint32_t mBaseVertexLocation; // A value which is added to each index before reading a vertex from the vertex buffer
				uint32_t mStartInstanceLocation;
			} mDrawIndexedInstanced;
			struct
			{
				const IBuffer* mBuffer;
				uint64_t mBufferOffset;
			} mDrawArrayIndirect, mDrawIndexedIndirect;
		};
	};

	class IRenderContext
	{
	public:
		virtual IRenderPipelineState* GetPipelineState() const = 0;
		virtual void SetPipelineState(IRenderPipelineState* state) = 0;
		virtual void Draw(const DrawAttribs& attribs) = 0;
		virtual void Flush(bool wait = false) = 0;
	};
}
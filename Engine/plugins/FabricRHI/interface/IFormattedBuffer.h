#pragma once

#include "IBuffer.h"

namespace ZHU
{
	enum class EBufferType
	{
		Undefined,
		Raw,
		Typed,
		Structured,
		AppendStructuredBuffer,  // The append buffer must be a structured buffer
		ConsumeStructuredBuffer, // The consume buffer must be a structured buffer
	};

	class IFormattedBuffer : public IBuffer
	{
	public:
		using StructuredType = MetaTable;

		virtual EBufferType GetBufferType() const = 0;
		virtual size_t GetElementCount() const = 0;
		virtual size_t GetElementSize() const = 0;

		virtual EResourceFormat GetTypedFormat() const = 0; // For typed buffer
		virtual StructuredType GetStructuredType() const = 0; // For structured buffer

		virtual void SetTypedFormat(EResourceFormat format) = 0;
		virtual void SetStructuredType(const StructuredType& type) = 0;
	};
}
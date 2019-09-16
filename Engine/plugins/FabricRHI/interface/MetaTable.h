#pragma once

#include "RHIUtils.h"
#include <type_traits>

namespace ZHU
{
	class BasicType;
	class ArrayType;
	class ClassType;
	
	class RHI_API MetaTable final
	{
		struct PrivateData;
	public:
		MetaTable();
		~MetaTable();

	private:
		PrivateData* mData = nullptr;
	};

	class RHI_API BasicType final
	{
		struct PrivateData;
	public:
		BasicType();
		~BasicType();

	private:
		PrivateData* mData = nullptr;
	};

	class RHI_API ArrayType final
	{
		struct PrivateData;
	public:
		ArrayType();
		~ArrayType();

	private:
		PrivateData* mData = nullptr;
	};

	class RHI_API ClassType final
	{
		struct PrivateData;
	public:
		ClassType();
		~ClassType();

	private:
		PrivateData* mData = nullptr;
	};
}
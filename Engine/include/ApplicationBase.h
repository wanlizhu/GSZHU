#pragma once

#include "Framework.h"
#include "Utils/Object.h"

namespace GS
{
	class GS_API ApplicationBase 
	{
	public:
		using SharedPtr = std::shared_ptr<ApplicationBase>;
		using ConstSharedPtr = std::shared_ptr<const ApplicationBase>;

		ApplicationBase();
		ApplicationBase(const ApplicationBase&) = delete;
		ApplicationBase& operator=(const ApplicationBase&) = delete;
		virtual ~ApplicationBase();

	private:
		
	};
}
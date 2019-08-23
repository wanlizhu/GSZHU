#pragma once

#include <vector>
#include <array>
#include <functional>
#include "Framework.h"

namespace GS
{
	class GS_API IProgressBar : public Object,
		                        public inherit_shared_from_this<Object, IProgressBar>
	{
	public:
		using inherit_shared_from_this<Object, IProgressBar>::shared_from_this;
		using SharedPtr = std::shared_ptr<IProgressBar>;
		using SharedConstPtr = std::shared_ptr<const IProgressBar>;
		struct Desc
		{
			WindowHandle ParentWindow = 0;
			std::string Title = "Progress Bar";
			std::string Message = "Please wait...";
			bool IsDecorated = true;
			bool IsMovable = true;
			bool IsTopMost = true;

			std::array<int, 2> ValueRange;
			std::function<void()> Callback;
		};

		static SharedPtr Create(const Desc& desc);
		virtual ~IProgressBar();
		
		virtual void Show() = 0;
		virtual void Quit() = 0;
		virtual bool MoveForward(int delta) = 0;
		virtual void SetMessage(const std::string& msg) = 0;
		virtual bool IsCompleted() const = 0;

	protected:
		IProgressBar(const std::string& name);
	};
}
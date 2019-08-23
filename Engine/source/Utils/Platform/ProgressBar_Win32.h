#pragma once

#include "Utils/Platform/IProgressBar.h"
#include "Framework.h"
#include <thread>
#include <atomic>

namespace GS
{
	namespace local
	{
		LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
	}

	class ProgressBar_Win32 : public IProgressBar
		                    , public inherit_shared_from_this<IProgressBar, ProgressBar_Win32>
	{
	public:
		friend LRESULT CALLBACK local::WindowProc(HWND, UINT, WPARAM, LPARAM);
		using inherit_shared_from_this<IProgressBar, ProgressBar_Win32>::shared_from_this;
		using SharedPtr = std::shared_ptr<ProgressBar_Win32>;
		using SharedConstPtr = std::shared_ptr<const ProgressBar_Win32>;

		static SharedPtr Create(const IProgressBar::Desc& desc);
		virtual ~ProgressBar_Win32();

		virtual void Show() override;
		virtual void Quit() override;
		virtual bool MoveForward(int delta) override;
		virtual void SetMessage(const std::string& msg) override;
		virtual bool IsCompleted() const override;

	private:
		ProgressBar_Win32(const std::string& name);
		void CreateWidgets();
		
	private:
		HWND mhWindow = NULL;
		HWND mhLabel = NULL;
		HWND mhProgressBar = NULL;
		std::thread mThread;

		IProgressBar::Desc mDesc;
		bool mIsCompleted = false;
		int mAccumValue = 0;
	};
}
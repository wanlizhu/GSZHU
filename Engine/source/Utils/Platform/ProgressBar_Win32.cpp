#include "ProgressBar_Win32.h"
#include "resource.h"
#include <CommCtrl.h>
#include <Windows.h>

#pragma comment(lib, "ComCtl32.lib")

namespace GS
{
	namespace local
	{
		static const char* _kClassName = "ProgressBar_Win32";

		LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			auto winPtr = (GS::ProgressBar_Win32*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
			assert(winPtr != nullptr);

			switch (msg)
			{
			case WM_CREATE:
				winPtr->CreateWidgets();
				break;

			default:
				return ::DefWindowProcA(hWnd, msg, wParam, lParam);
			}

			return  0;
		}
	} // namespace local

	ProgressBar_Win32::SharedPtr ProgressBar_Win32::Create(const IProgressBar::Desc& desc)
	{
		WNDCLASSEXA wc;
		wc.cbSize = sizeof(WNDCLASSEXA);
		wc.style = 0;
		wc.lpfnWndProc = (WNDPROC)local::WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = (HINSTANCE)GetWindowLongPtrA(desc.ParentWindow, GWLP_HINSTANCE);
		wc.hIcon = ::LoadIconA(wc.hInstance, MAKEINTRESOURCEA(IDI_ICON_WAIT));
		wc.hIconSm = ::LoadIconA(wc.hInstance, MAKEINTRESOURCEA(IDI_ICON_WAIT));
		wc.hCursor = ::LoadCursorA(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = local::_kClassName;

		if (!::RegisterClassExA(&wc))
		{
#if LOG_ENABLED
			LOG(ERROR) << "Error(" << GetLastError() << ")Failed to register window class: \"" << local::_kClassName << "\"";
#endif
			return nullptr;
		}

		HWND handle = ::CreateWindowExA(WS_EX_CLIENTEDGE,
										local::_kClassName,
										desc.Title.c_str(),
										WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
										CW_USEDEFAULT,
										CW_USEDEFAULT,
										100,
										40,
										desc.ParentWindow,
										NULL,
										wc.hInstance,
										NULL);
		if (!handle)
		{
#if LOG_ENABLED
			LOG(ERROR) << "Error(" << GetLastError() << ") Failed to create window: \"" << local::_kClassName << "\"";
#endif
			return nullptr;
		}

		static int _id = 0;
		SharedPtr shared(new ProgressBar_Win32("ProgressBar_Win32 " + std::to_string(_id++)));
		shared->mhWindow = handle;
		shared->mDesc = desc;
		
		return shared;
	}

	ProgressBar_Win32::ProgressBar_Win32(const std::string& name)
		: IProgressBar(name)
	{}

	ProgressBar_Win32::~ProgressBar_Win32()
	{
		mThread.detach();
	}

	void ProgressBar_Win32::Show()
	{
		auto _ThreadFunc = [&]()->WPARAM {
			::ShowWindow(mhWindow, SW_SHOW);
			::UpdateWindow(mhWindow);

			mIsCompleted = false;
			MSG msg;
			while (::GetMessageA(&msg, NULL, 0, 0))
			{
				::TranslateMessage(&msg);
				::DispatchMessageA(&msg);
			}
			mIsCompleted = true;

			return msg.wParam;
		};

		mThread = std::thread(_ThreadFunc);
	}

	void ProgressBar_Win32::Quit()
	{
		if (mIsCompleted)
			return;
		::SendMessageA(mhWindow, WM_CLOSE, 0, 0);
	}

	bool ProgressBar_Win32::MoveForward(int delta)
	{
		if (mIsCompleted)
			return false;

		::SendMessageA(mhProgressBar, PBM_DELTAPOS, delta, 0);
		mAccumValue += delta;

		if (mAccumValue >= mDesc.ValueRange[1])
		{
			mIsCompleted = true;
			if (mDesc.Callback)
				mDesc.Callback();
			Quit();
		}

		return true;
	}

	void ProgressBar_Win32::SetMessage(const std::string& msg)
	{
		if (mIsCompleted || mhLabel == NULL)
			return;
		::SetWindowTextA(mhLabel, msg.c_str());
	}

	bool ProgressBar_Win32::IsCompleted() const
	{
		return mIsCompleted;
	}

	void ProgressBar_Win32::CreateWidgets()
	{
		assert(mhWindow != NULL);

		mhLabel = ::CreateWindowExA(0, "static", "ProgressBar_Win32", 
									WS_CHILD | WS_VISIBLE | WS_TABSTOP,
									0, 0, CW_USEDEFAULT, CW_USEDEFAULT, 
									mhWindow, (HMENU)501,
									(HINSTANCE)GetWindowLongPtrA(mhWindow, GWLP_HINSTANCE),
									NULL);
		if (!mhLabel)
		{
#if LOG_ENABLED
			LOG(ERROR) << "Error(" << GetLastError() << ") Failed to create Progress Bar Label.";
#endif
			return;
		}
		::SetWindowTextA(mhLabel, mDesc.Message.c_str());

		RECT rcClient;
		GetClientRect(mhWindow, &rcClient);
		int cyVScroll = GetSystemMetrics(SM_CYVSCROLL);

		mhProgressBar = ::CreateWindowExA(0, 
										  PROGRESS_CLASS,
										  (LPSTR)NULL,
										  WS_VISIBLE | WS_CHILD,
										  rcClient.left, 
										  rcClient.bottom - cyVScroll,
										  rcClient.right,
										  cyVScroll,
										  mhWindow,
										  (HMENU)IDPB_PROGRESS_BAR,
										  (HINSTANCE)GetWindowLongPtrA(mhWindow, GWLP_HINSTANCE),
										  NULL);
		if (!mhProgressBar)
		{
#if LOG_ENABLED
			LOG(ERROR) << "Error(" << GetLastError() << ") Failed to create Progress Bar.";
#endif
			return;
		}

		::SendMessageA(mhProgressBar, PBM_SETRANGE32, mDesc.ValueRange[0], mDesc.ValueRange[1]);
		::SendMessageA(mhProgressBar, PBM_SETSTEP, (WPARAM)1, 0);
	}
}
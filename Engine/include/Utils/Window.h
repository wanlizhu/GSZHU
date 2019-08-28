#pragma once

#include "Framework.h"
#include "Utils/Object.h"
#include <any>
#include <memory>
#include <string>
#include <list>
#include <array>
#include <unordered_map>
#include <functional>

struct GLFWwindow;

namespace GS
{
	struct InputModifiers
	{
		bool Ctrl = false;
		bool Shift = false;
		bool Alt = false;
	};
	
	struct KeyboardEvent
	{
		enum class EType 
		{
			Down,
			Up,
			Char,
		};
		
		EType Type;
		char Key;
		InputModifiers Mods;
		uint32_t CodePoint = 0;
	};
	
	struct MouseEvent
	{
		enum class EType 
		{
			None,
			LeftDown,
			LeftUp,
			MiddleDown,
			MiddleUp,
			RightDown,
			RightUp,
			Move,
			Wheel,
		};
		
		EType Type = EType::None;
		InputModifiers Mods;
		std::array<int, 2> Position;
		std::array<int, 2> WheelDelta;
	};


    class GS_API Window : public Object,
		                  public inherit_shared_from_this<Object, Window>
    {
		GS_OBJECT
    public:
		using inherit_shared_from_this<Object, Window>::shared_from_this;
		using SharedPtr = std::shared_ptr<Window>;
		using SharedConstPtr = std::shared_ptr<const Window>;
		using MSG_ID = uint32_t;
		using MSG_FUNC = std::function<void(WindowHandle, uint32_t, uint32_t)>;
		enum class EBackend 
		{
			None,
			OpenGL,
			Vulkan,
			D3D12
		};
		struct Desc 
		{
			EBackend Backend = EBackend::None;
			std::wstring Title = L"Untitled";
			std::array<int, 2> Position = { -1 };
			std::array<int, 2> Size = { -1 };
			bool IsFullScreen = false;
			bool IsResizable = true;
			bool AcceptDropFiles = true;
		};
		class ICallbacks
		{
		public:
			virtual void OnCreate() = 0;
			virtual void OnDestroy() = 0;
			virtual void OnResize(int width, int height) = 0;
			virtual void OnUpdate() = 0;
			virtual void OnRender() = 0;

			virtual void OnKeyboardEvent(const KeyboardEvent& event) = 0;
			virtual void OnMouseEvent(const MouseEvent& event) = 0;
			virtual void OnDropFile(const std::vector<std::wstring>& paths) = 0;
		};

        static std::array<int, 2> DefaultPos();
        static std::array<int, 2> DefaultSize();
		static SharedPtr Create(const Window::Desc& desc, ICallbacks* callbacks);

		virtual ~Window();
		void SetCallbacks(ICallbacks* callbacks);
        void SetTitle(const std::wstring& title);
        void SetFullScreen(bool enabled = true) ;
        void SetSize(int width, int height);
        void MoveTo(int x, int y);
		void SetMessageHook(MSG_ID msg, const MSG_FUNC& func);
        void Hide();
        void Show();
        void Destroy();
		void MessageLoop();

        std::wstring GetTitle() const;
        std::array<int, 2> GetPosition() const;
        std::array<int, 2> GetSize() const;
		WindowHandle GetWindowHandle() const;
		std::array<float, 2> GetMouseScale() const;
		ICallbacks* GetCallbacks() const;
        
    protected:
		static void Initialize();
		static void SetWindowHints(const Desc& desc);
		Window(const std::wstring& name, Window::ICallbacks* callbacks);
		void ComputeMouseScale();
		
    private:
        GLFWwindow* mpWindow = nullptr;
		ICallbacks* mpCallbacks = nullptr;
		std::unordered_map<MSG_ID, MSG_FUNC> mMessageHooks;

		std::array<float, 2> mMouseScale = { 1.0f, 1.0f };
		EBackend mBackend = EBackend::None;
    };
      
}
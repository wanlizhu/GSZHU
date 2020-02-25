#pragma once
#ifdef PLATFORM_WINDOWS

#include "Core/Common.h"
#include "Utils/NonCopyable.h"
#include <Windows.h>
#include <stack>

namespace Wanli
{
    enum class EMenuID
    {
        File_New,
        File_Open,
        File_Create,
        File_Save,
        File_SaveAs,
        File_EditPerference,
        File_Exit,
    };

    class Menu : public NonCopyable
    {
    public:
        using Callback = std::function<void()>;

        static LRESULT CALLBACK MessageProc(HWND, UINT, WPARAM, LPARAM);
        
        Menu() = default;
        Menu(Menu&& rhs);
        Menu& operator=(Menu&& rhs);
        virtual ~Menu();

        void BeginHorizontalMenu(HWND hwnd);
        void BeginVerticalMenu(const String& name);
        void AddMenuItem(const String& name, const Callback& callback);
        void AddMenuItem(const String& name, EMenuID id);
        void AddSeparator();
        void EndVerticalMenu();
        void EndHorizontalMenu();
        
        HMENU GetHandle() const;
        void Destroy();

    protected:
        HWND mWindow = NULL;
        HMENU mMenu = NULL;
        HashMap<SHORT, Callback> mCallbackMap;

    private:
        static HashMap<HWND, Menu*> smInstanceMap;
        std::stack<std::pair<HMENU, String>> mCreateInfoStack;
    };
}

#endif
#ifdef PLATFORM_WINDOWS
#include "Application/Windows/Menu.h"
#include "Events/EventModule.h"

namespace Wanli
{
#define MENU_ID_BEGIN 4001

    SHORT GetNextMenuID()
    {
        static SHORT nextId = MENU_ID_BEGIN;
        return nextId++;
    }

    HashMap<HWND, Menu*> Menu::smInstanceMap;

    LRESULT CALLBACK Menu::MessageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        auto it = smInstanceMap.find(hwnd);
        if (it != smInstanceMap.end())
        {
            Menu* instance = it->second;
            if (msg == WM_COMMAND)
            {
                SHORT id = LOWORD(wParam);
                auto it2 = instance->mCallbackMap.find(id);
                if (it2 != instance->mCallbackMap.end())
                {
                    (it2->second)();
                }
            }
        }

        return 0;
    }

    Menu::Menu(Menu&& rhs)
    {
        mWindow = rhs.mWindow;
        mMenu = rhs.mMenu;
        mCallbackMap = rhs.mCallbackMap;

        rhs.mWindow = NULL;
        rhs.mMenu = NULL;
        rhs.mCallbackMap.clear();
    }

    Menu& Menu::operator=(Menu&& rhs)
    {
        mWindow = rhs.mWindow;
        mMenu = rhs.mMenu;
        mCallbackMap = rhs.mCallbackMap;

        rhs.mWindow = NULL;
        rhs.mMenu = NULL;
        rhs.mCallbackMap.clear();

        return *this;
    }

    Menu::~Menu()
    {
        Destroy();
    }

    void Menu::BeginHorizontalMenu(HWND hwnd)
    {
        mWindow = hwnd;
        mMenu = ::CreateMenu();
        mCreateInfoStack.push(std::make_pair(mMenu, ""));
    }

    void Menu::BeginVerticalMenu(const String& name)
    {
        HMENU subMenu = ::CreateMenu();
        mCreateInfoStack.push(std::make_pair(subMenu, name));
    }

    void Menu::AddMenuItem(const String& name, const Callback& callback)
    {
        SHORT menuId = GetNextMenuID();
        ::AppendMenuA(mCreateInfoStack.top().first, MF_STRING, menuId, name.c_str());
        mCallbackMap[menuId] = callback;
    }

    void Menu::AddMenuItem(const String& name, EMenuID id)
    {
        SHORT menuId = GetNextMenuID();
        ::AppendMenuA(mCreateInfoStack.top().first, MF_STRING, menuId, name.c_str());
        mCallbackMap[menuId] = [=]() {
            Event::Create(EVT_WindowMenu, id)->PostEvent();
        };
    }

    void Menu::AddSeparator()
    {
        ::AppendMenuA(mCreateInfoStack.top().first, MF_SEPARATOR, NULL, NULL);
    }

    void Menu::EndVerticalMenu()
    {
        HMENU subMenu = mCreateInfoStack.top().first;
        String subMenuName = mCreateInfoStack.top().second;
        mCreateInfoStack.pop();

        HMENU parent = mCreateInfoStack.top().first;
        ::AppendMenuA(parent, MF_POPUP, (UINT_PTR)subMenu, subMenuName.c_str());
    }

    void Menu::EndHorizontalMenu()
    {
        ::SetMenu(mWindow, mMenu);
        smInstanceMap[mWindow] = this;
    }

    HMENU Menu::GetHandle() const
    {
        return mMenu;
    }

    void Menu::Destroy()
    {
        if (mMenu)
        {
            ::DestroyMenu(mMenu);
            mMenu = NULL;
            mWindow = NULL;
        }
    }
}

#endif
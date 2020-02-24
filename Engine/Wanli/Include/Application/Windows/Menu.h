#pragma once
#ifdef PLATFORM_WINDOWS

#include "Core/BasicTypes.h"
#include "Utilities/NonCopyable.h"
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

        View_Wireframe,
        View_Solid,
        View_Debug,
        View_Select_Point,
        View_Select_Line,
        View_Select_Face,
        View_Select_Mesh,
        View_Select_Object,

        Edit_Copy,
        Edit_Cut,
        Edit_Paste,
        Edit_AddText,
        Edit_AddPrimitive_Point,
        Edit_AddPrimitive_Line,
        Edit_AddPrimitive_Plane,
        Edit_AddPrimitive_Cube,
        Edit_AddPrimitive_Sphere,
        Edit_AddPrimitive_Torus,
        Edit_AddModel_Bunny,
        Edit_AddModel_Dragon,
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
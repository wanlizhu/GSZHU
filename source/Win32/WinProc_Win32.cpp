#include "Win32/WinProc_Win32.h"
#include "Win32/CApplication_Win32.h"
#include "Win32/CWindow_Win32.h"

namespace GSZHU {
    namespace LOCAL {
        void SetMouseButtonEvent(UINT message, WPARAM wParam, LPARAM lParam, SMouseButtonEvent* event) {
            switch (message) {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                case WM_LBUTTONDBLCLK:
                {
                    event->button = EMouseButton::Left;
                }
                break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                case WM_RBUTTONDBLCLK:
                {
                    event->button = EMouseButton::Right;
                }
                break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                case WM_MBUTTONDBLCLK:
                {
                    event->button = EMouseButton::Middle;
                }
                break;
                default:
                event->button = EMouseButton::None;
            }

            event->state = EKeyState::Down;
            event->mods.shift = (wParam & MK_SHIFT) != 0;
            event->mods.control = (wParam & MK_CONTROL) != 0;
            event->mods.alt = ::GetAsyncKeyState(VK_MENU);

            event->x = ((int)(short)LOWORD(lParam));
            event->y = ((int)(short)HIWORD(lParam));
        }
    }

    LRESULT CALLBACK WinProc_Win32(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) {
        std::shared_ptr<CWindow_Win32> window;
        std::weak_ptr<IWindow> weak = CApplication_Win32::GetInstance()->FindWindowByHandle(windowHandle);

        if (window = std::dynamic_pointer_cast<CWindow_Win32>(weak.lock())) {
            switch (message) {
                case WM_PAINT:
                {
                    static UINT64 sFrameCounter = 0;
                    sFrameCounter++;

                    static SWindowUpdateEvent sUpdateEvent;
                    window->mWindowClock.Tick(CWindowClock::UpdateClock);
                    sUpdateEvent.frameNumber = sFrameCounter;
                    sUpdateEvent.deltaTime = window->GetWindowClock().GetDelta(CWindowClock::UpdateClock);
                    sUpdateEvent.totalTime = window->GetWindowClock().GetTotal(CWindowClock::UpdateClock);
                    window->OnEvent(sUpdateEvent);

                    static SWindowUpdateEvent sRenderEvent;
                    window->mWindowClock.Tick(CWindowClock::RenderClock);
                    sRenderEvent.frameNumber = sFrameCounter;
                    sRenderEvent.deltaTime = window->GetWindowClock().GetDelta(CWindowClock::RenderClock);
                    sRenderEvent.totalTime = window->GetWindowClock().GetTotal(CWindowClock::RenderClock);
                    window->OnEvent(sRenderEvent);
                }
                break;

                case WM_SYSKEYDOWN:
                case WM_KEYDOWN:
                {
                    MSG charMsg;
                    UINT unicode = 0;
                    UINT scanCode = 0;

                    // For printable characters, the next message will be WM_CHAR.
                    // This message contains the character code we need to send the KeyPressed event.
                    if (::PeekMessage(&charMsg, windowHandle, 0, 0, PM_NOREMOVE) && charMsg.message == WM_CHAR) {
                        ::GetMessage(&charMsg, windowHandle, 0, 0);
                        unicode = static_cast<unsigned int>(charMsg.wParam);
                        scanCode = (lParam & 0x00FF0000) >> 16;

                        if (charMsg.wParam > 0 && charMsg.wParam < 0x10000) {
                            //ImGui::GetIO().AddInputCharacter((unsigned short)CharMsg.wParam);
                        }
                    }

                    static SKeyEvent sKeyEvent;
                    sKeyEvent.key = (EKeyCode)wParam;
                    sKeyEvent.unicode = unicode;
                    sKeyEvent.state = EKeyState::Down;
                    sKeyEvent.mods.shift = (::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
                    sKeyEvent.mods.control = (::GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
                    sKeyEvent.mods.alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

                    window->OnEvent(sKeyEvent);
                } 
                break;

                case WM_SYSKEYUP:
                case WM_KEYUP:
                {
                    UINT unicode = 0;
                    UINT scanCode = (lParam & 0x00FF0000) >> 16;

                    // Determine which key was released by converting the key code and the scan code
                    // to a printable character (if possible).
                    unsigned char keyboardState[256];
                    auto success = ::GetKeyboardState(keyboardState);
                    wchar_t translatedChars[4];
                    if (int result = ::ToUnicodeEx(static_cast<UINT>(wParam), scanCode, keyboardState, translatedChars, 4, 0, NULL) > 0) {
                        unicode = translatedChars[0];
                    }

                    static SKeyEvent sKeyEvent;
                    sKeyEvent.key = (EKeyCode)wParam;
                    sKeyEvent.unicode = unicode;
                    sKeyEvent.state = EKeyState::Up;
                    sKeyEvent.mods.shift = (::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
                    sKeyEvent.mods.control = (::GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
                    sKeyEvent.mods.alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

                    window->OnEvent(sKeyEvent);
                } 
                break;

                // The default window procedure will play a system notification sound 
                // when pressing the Alt+Enter keyboard combination if this message is not handled.
                case WM_SYSCHAR:
                break;

                case WM_MOUSEMOVE:
                {
                    if (window->mLastMousePos[0] == INT_MAX) {
                        window->mLastMousePos[0] = ((int)(short)LOWORD(lParam));
                        window->mLastMousePos[1] = ((int)(short)HIWORD(lParam));
                    }

                    static SMouseMotionEvent sMotionEvent;
                    sMotionEvent.x = ((int)(short)LOWORD(lParam));
                    sMotionEvent.y = ((int)(short)HIWORD(lParam));
                    sMotionEvent.xOffset = sMotionEvent.x - window->mLastMousePos[0];
                    sMotionEvent.yOffset = sMotionEvent.y - window->mLastMousePos[1];
                    sMotionEvent.buttons.left = (wParam & MK_LBUTTON) != 0;
                    sMotionEvent.buttons.right = (wParam & MK_RBUTTON) != 0;
                    sMotionEvent.buttons.middle = (wParam & MK_MBUTTON) != 0;
                    sMotionEvent.mods.shift = (wParam & MK_SHIFT) != 0;
                    sMotionEvent.mods.control = (wParam & MK_CONTROL) != 0;
                    sMotionEvent.mods.alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

                    window->mLastMousePos[0] = sMotionEvent.x;
                    window->mLastMousePos[1] = sMotionEvent.y;
                    
                    window->OnEvent(sMotionEvent);
                }
                break;

                case WM_LBUTTONDOWN:
                case WM_RBUTTONDOWN:
                case WM_MBUTTONDOWN:
                {
                    static SMouseButtonEvent sButtonEvent;
                    LOCAL::SetMouseButtonEvent(message, wParam, lParam, &sButtonEvent);
                    sButtonEvent.state = EKeyState::Down;
                    window->OnEvent(sButtonEvent);
                }
                break;

                case WM_LBUTTONUP:
                case WM_RBUTTONUP:
                case WM_MBUTTONUP:
                {
                    static SMouseButtonEvent sButtonEvent;
                    LOCAL::SetMouseButtonEvent(message, wParam, lParam, &sButtonEvent);
                    sButtonEvent.state = EKeyState::Up;
                    window->OnEvent(sButtonEvent);
                }
                break;

                case WM_MOUSEWHEEL:
                {
                    static SMouseWheelEvent sWheelEvent;
                    sWheelEvent.delta = ((int)(short)HIWORD(wParam)) / (float)WHEEL_DELTA;
                    sWheelEvent.buttons.left = (((short)LOWORD(wParam)) & MK_LBUTTON) != 0;
                    sWheelEvent.buttons.right = (((short)LOWORD(wParam)) & MK_RBUTTON) != 0;
                    sWheelEvent.buttons.middle = (((short)LOWORD(wParam)) & MK_MBUTTON) != 0;
                    sWheelEvent.mods.shift = (((short)LOWORD(wParam)) & MK_SHIFT) != 0;
                    sWheelEvent.mods.control = (((short)LOWORD(wParam)) & MK_CONTROL) != 0;
                    sWheelEvent.mods.alt = ::GetAsyncKeyState(VK_MENU);

                    // Convert the screen coordinates to client coordinates.
                    POINT point;
                    point.x = ((int)(short)LOWORD(lParam));
                    point.y = ((int)(short)HIWORD(lParam));
                    ::ScreenToClient(windowHandle, &point);
                    sWheelEvent.x = point.x;
                    sWheelEvent.y = point.y;

                    window->OnEvent(sWheelEvent);
                }
                break;

                case WM_SIZE:
                {
                    static SWindowResizeEvent resizeEvent;
                    resizeEvent.width = ((int)(short)LOWORD(lParam));
                    resizeEvent.height = ((int)(short)HIWORD(lParam));

                    window->OnEvent(resizeEvent);
                }
                break;

                case WM_DESTROY:
                {
                    static SWindowDestroyEvent sDestroyEvent;
                    window->OnEvent(sDestroyEvent);
                }
                break;
            }
        }

        return ::DefWindowProc(windowHandle, message, wParam, lParam);
    }
}
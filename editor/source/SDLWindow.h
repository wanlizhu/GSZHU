#pragma once

#include <string>
#include <SDL.h>
#include <SDL_syswm.h>

class SDLWindow {
public:
    SDLWindow();
    SDLWindow(SDL_Window* win);
    SDLWindow(const std::string& title, int width, int height, uint32_t flags);
    SDLWindow(const SDLWindow&) = delete;
    SDLWindow& operator=(const SDLWindow&) = delete;
    virtual ~SDLWindow();

    bool IsValid() const;
    bool IsActive() const;
    void Close();
    SDL_Window* GetSDLHandle() const;
    HWND GetWin32Handle() const;
    uint32_t GetWindowID() const;

protected:
    SDL_Window* mpSDLHandle = nullptr;
    HWND mpWin32Handle = nullptr;
    uint32_t mWindowID = 0;
};
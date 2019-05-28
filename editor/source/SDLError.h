#pragma once

#include <exception>
#include <string>
#include <SDL.h>

class SDLError : public std::exception {
public:
    SDLError()
        : mMessage(SDL_GetError())
    {}
    SDLError(const std::string& msg)
        : mMessage(msg)
    {}
    virtual ~SDLError() noexcept {}

    virtual const char* what() const noexcept {
        return mMessage.c_str();
    }

private:
    std::string mMessage;
};
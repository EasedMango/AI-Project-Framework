#pragma once
#include <SDL_video.h>
#include <string>

#include "SDLTypes.h"

class Window
{
private:
    int width, height;
    SDLWindowPtr window;
    SDLSurfacePtr screenSurface;

public:
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    Window();
    ~Window();
    bool OnCreate(std::string name_, int width_, int height_);
    void OnDestroy();

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    SDL_Window* GetWindow() const { return window.get(); }

};


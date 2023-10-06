#include "Window.h"

#include "Debug.h"

Window::Window()
{
}

Window::~Window()
{
}

bool Window::OnCreate(std::string name_, int width_, int height_)
{
    this->width = width_;
    this->height = height_;
    window = SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        Debug::FatalError(std::string("SDL_Error: ") + SDL_GetError());
        return false;
    }
    screenSurface = SDL_GetWindowSurface(window);
    if (screenSurface == nullptr) {
        Debug::FatalError( std::string("SDL_Error: ") + SDL_GetError()) ;
        return false;
    }

    return true;
}

void Window::OnDestroy()
{
    //Destroy surface
    if (screenSurface) {
        SDL_FreeSurface(screenSurface);
        screenSurface = nullptr;
    }
    // Destroy the window
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }


}

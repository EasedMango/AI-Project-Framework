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
    window = SDLWindowPtr (SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN));

    if (window == nullptr) {
        Debug::FatalError(std::string("SDL_Error: ") + SDL_GetError());
        return false;
    }
    screenSurface = SDLSurfacePtr(SDL_GetWindowSurface(window.get()));
    if (screenSurface == nullptr) {
        Debug::FatalError( std::string("SDL_Error: ") + SDL_GetError()) ;
        return false;
    }

    return true;
}

void Window::OnDestroy()
{

}

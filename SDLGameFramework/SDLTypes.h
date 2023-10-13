#pragma once
#include <memory>

struct SDL_Renderer;

struct SDL_Window;

struct SDL_Texture;

struct SDL_Surface;



struct SDLDeleter
{
	void operator()(SDL_Texture* texture) const;

	void operator()(SDL_Renderer* renderer) const;

	void operator()(SDL_Window* window) const;

	void operator()(SDL_Surface* surface) const;
};

using SDLRendererPtr = std::unique_ptr<SDL_Renderer, SDLDeleter>;

using SDLWindowPtr = std::unique_ptr<SDL_Window, SDLDeleter>;

using SDLTexturePtr = std::unique_ptr<SDL_Texture, SDLDeleter>;

using SDLSurfacePtr = std::unique_ptr<SDL_Surface, SDLDeleter>;
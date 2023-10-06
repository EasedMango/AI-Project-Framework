#include "Renderer.h"

#include <SDL_image.h>

#include "Window.h"
#include "glm/vec3.hpp"

void Renderer::Init(const Ref<Window >&window)
{
	this->window = window;
	renderer = SDL_CreateRenderer(window->GetWindow(), -1, SDL_RENDERER_ACCELERATED);
}

void Renderer::GiveCamera(const Ref<Camera>& camera_)
{
	camera = camera_;
}

void Renderer::BeginFrame() const
{
	SDL_RenderClear(renderer);
}

void Renderer::EndFrame() const
{
	SDL_RenderPresent(renderer);
}

Sprite Renderer::CreateSprite(const std::string& filename, int scale)
{
	auto it = textureMap.find(filename);
	ID textureID;

	if (it != textureMap.end())
	{
		// Texture already exists, reuse it.
		textureID = it->second;
	}
	else
	{

		std::string path = "Sprites/" + filename;
		// Load texture and store it.
		SDL_Texture* sdlTexture = IMG_LoadTexture(renderer, path.c_str());
		if(sdlTexture == nullptr)
		{
			SDL_Log("Failed to load texture: %s - %s", filename.c_str(), IMG_GetError());
			;
			return Sprite{ 0, 0 };
		}
		int width, height;
		SDL_QueryTexture(sdlTexture, nullptr, nullptr, &width, &height);

		textureID = textures.size();
		textures.emplace_back(new Texture{ sdlTexture, width, height });
		textureMap[filename] = textureID;
	}

	return Sprite{ textureID, scale };
}

void Renderer::RenderSprite(Sprite sprite, const glm::vec3& position, const float& angle) const
{
	auto& texture = textures[sprite.textureID];
	// convert the position from game coords to screen coords
	SDL_Rect square;
	glm::vec3 screenCoords;
	float    w, h;

	// convert the position from game coords to screen coords
	screenCoords = camera->GetProjectionMatrix() * glm::vec4(position,1.f);
	w = texture->width * sprite.scale;
	h = texture->height * sprite.scale;

	square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	square.y = static_cast<int>(screenCoords.y - 0.5f * h);
	square.w = static_cast<int>(w);
	square.h = static_cast<int>(h);

	// Convert character orientation from radians to degrees.
	SDL_RenderCopyEx(renderer, texture->texture, nullptr, &square,
		angle, nullptr, SDL_FLIP_NONE);
	

}

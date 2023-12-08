#include "Renderer.h"

#include <SDL_image.h>

#include "GUI.h"
#include "Window.h"
#include "glm/vec3.hpp"


Renderer::Renderer(Window* window) : window(window)
{
	renderer =  SDLRendererPtr( SDL_CreateRenderer(window->GetWindow(), -1, SDL_RENDERER_ACCELERATED));
	imGui = std::make_unique<GUI>(*this);
}

void Renderer::GiveCamera(const Ref<Camera>& camera_)
{
	camera = camera_;
}

void Renderer::BeginFrame() const
{
	SDL_RenderClear(renderer.get());
	
	imGui->BeginFrame(window->GetWindow());

}

void Renderer::EndFrame() const
{
	imGui->EndFrame(renderer);
	SDL_RenderPresent(renderer.get());
}

Sprite Renderer::CreateSprite(const std::string& filename, int scale,glm::vec4 colour)
{
	auto it = textureMap.find(filename);
	ID textureID;
	int width, height;
	if (textureMap.contains(filename))
	{
		// Texture already exists, reuse it.
		textureID = it->second;
		width = textures[textureID]->width;
		height = textures[textureID]->height;
	}
	else
	{

		std::string path = "Sprites/" + filename;
		// Load texture and store it.
		SDLTexturePtr sdlTexture = SDLTexturePtr (IMG_LoadTexture(renderer.get(), path.c_str()));
		if(sdlTexture == nullptr)
		{
			SDL_Log("Failed to load texture: %s - %s", filename.c_str(), IMG_GetError());
			;
			return Sprite{ 0, 0 };
		}

		SDL_QueryTexture(sdlTexture.get(), nullptr, nullptr, &width, &height);

		textureID = textures.size();
		textures.emplace_back(std::make_unique<Texture>(Texture{std::move(sdlTexture), width, height}));
		textureMap[filename] = textureID;
	}

	return Sprite{ textureID, scale,colour,width,height };
}

void Renderer::RenderSprite(Sprite sprite, const glm::vec3& position, const float& angle ) const
{
	auto& texture = textures[sprite.textureID];
	// convert the position from game coords to screen coords
	SDL_Rect square;
	glm::vec3 screenCoords;
	float    w, h;

	// convert the position from game coords to screen coords
	screenCoords = camera->GetProjectionMatrix() * camera->GetViewMatrix() * glm::vec4(position,1.f);


	w = (texture->width * sprite.scale);
	h = (texture->height * sprite.scale);

	square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	square.y = static_cast<int>(screenCoords.y - 0.5f * h);
	square.w = static_cast<int>(w);
	square.h = static_cast<int>(h);

	// Apply hue shift
	SDL_SetTextureColorMod(texture->texture.get(), sprite.colour.r, sprite.colour.g, sprite.colour.b);

	// Convert character orientation from radians to degrees.
	SDL_RenderCopyEx(renderer.get(), texture->texture.get(), nullptr, &square,
		angle, nullptr, SDL_FLIP_NONE);
	

}

void Renderer::RenderGrid()
{

}

void Renderer::RenderColoredRect(const glm::vec3& position, const glm::vec2& dimensions, const float& angle, const glm::vec4& color=glm::vec4(255,255,255, 255)) const
{
	SDL_Rect rect;
	glm::vec3 screenCoords;
	float w, h;

	// Convert the position from game coords to screen coords
	screenCoords = camera->GetProjectionMatrix() * glm::vec4(position, 1.f);
	w = dimensions.x;
	h = dimensions.y;

	rect.x = static_cast<int>(screenCoords.x - 0.5f * w);
	rect.y = static_cast<int>(screenCoords.y - 0.5f * h);
	rect.w = static_cast<int>(w);
	rect.h = static_cast<int>(h);

	SDL_SetRenderDrawColor(renderer.get(), color.r, color.g, color.b, color.a);

	// If you'd like to consider angle rotation, you may need to use advanced techniques
	// like transforming the points manually, as SDL_RenderFillRect doesn't support rotation.
	SDL_RenderFillRect(renderer.get(), &rect);
}

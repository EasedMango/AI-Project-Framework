#pragma once
#include <SDL_render.h>
#include <string>
#include "Camera.h"
#include "Common.h"
#include "glm/fwd.hpp"
#include <unordered_map>


class Window;

struct Texture
{
	SDL_Texture* texture;
	int width, height;
};

struct Sprite
{
	ID textureID;
	int scale;
};

class Renderer
{
private:
	SDL_Renderer* renderer;
	Ref<Window> window;
	Ref<Camera> camera;
	std::unordered_map<std::string, ID> textureMap;
	std::vector<std::unique_ptr<Texture>> textures;

	

protected:
	friend class Core;
	Renderer(): renderer(nullptr)
	{
	}
	;
	void Init(const Ref<Window >& window);
public:

	//delete copy constructor and assignment operator
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	void GiveCamera(const Ref<Camera>& camera_);
	static Renderer& GetInstance()
	{
		static auto* instance = new Renderer();
		return *instance;
	}
	Ref<Camera>& GetCamera() { return camera; }

	void BeginFrame() const;

	void EndFrame() const;

	Sprite CreateSprite(const std::string& filename, int scale = 1);

	 void RenderSprite(Sprite sprite, const glm::vec3& position, const  float& angle) const;


};

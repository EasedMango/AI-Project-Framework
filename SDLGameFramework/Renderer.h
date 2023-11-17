#pragma once
#include <SDL_render.h>
#include <string>
#include "Camera.h"
#include "Common.h"
#include "glm/fwd.hpp"
#include <unordered_map>
#include "SDLTypes.h"
#include "GUI.h"
class Window;

struct Texture
{
	SDLTexturePtr texture;
	int width, height;
};

struct Sprite
{
	ID textureID;
	int scale;
	glm::vec4 colour = { 255,255,255,255 };
	int width, height;
};



class Renderer
{
private:

	SDLRendererPtr renderer;
	Ref<Window> window;
	Ref<Camera> camera;
	ScopePtr<GUI> imGui;
	std::unordered_map<std::string, ID> textureMap;
	std::vector<ScopePtr<Texture>> textures;

	Renderer(const Ref<Window >& window);
protected:
	friend class Core;
	
	static Ref<Renderer> Create(const Ref<Window>& window) {
		return Ref<Renderer>(new Renderer(window));
	}
	friend class GUI;
	Ref<Window>& GetWindow()  { return window; }
	SDLRendererPtr& GetRenderer()  { return renderer; }
public:
	GUI& GetGUI() { return *imGui; }
	//delete copy constructor and assignment operator
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	void GiveCamera(const Ref<Camera>& camera_);



	//Creates a new camera but does not set it as the current camera
	[[nodiscard]] Ref<Camera> CreateCamera()
	{
		return Camera::Create(window);
	}

	Ref<Camera>& GetCurrentCamera() { return camera; }

	void BeginFrame() const;

	void EndFrame() const;

	Sprite CreateSprite(const std::string& filename, int scale = 1,glm::vec4 colour={255,255,255,255});


	/*
	 Render a sprite at a given position and angle
	 z value of position is used to determine render order (higher z values are rendered last)
	 */
	void RenderSprite(Sprite sprite, const glm::vec3& position, const  float& angle) const;

	void RenderGrid();

	void RenderColoredRect(const glm::vec3& position, const glm::vec2& dimensions, const float& angle, const glm::vec4& color) const;


};

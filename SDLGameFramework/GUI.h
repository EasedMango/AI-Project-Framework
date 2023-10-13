#pragma once
#include <SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "SDLTypes.h"

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

class Window;
class Renderer;

class GUI
{


	ImGuiIO& io;

	ImGuiIO& Setup();

public:
	inline static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	GUI(Renderer& renderer);

	~GUI();


	void BeginFrame(SDL_Window* window);
	// Poll and handle events (inputs, window resize, etc.)
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

	void PollEvents(SDL_Event& event);

	void Demo();

	void EndFrame(const SDLRendererPtr& renderer);
};


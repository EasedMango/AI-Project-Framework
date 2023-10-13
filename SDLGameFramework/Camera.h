#pragma once
#include <glm/mat4x4.hpp>

#include "Common.h"
class Window;

class Camera
{
private:
	Ref<Window> window;
	glm::mat4 projection;
	Camera(Ref<Window>& window);
protected:
	friend class Renderer;
	static Ref<Camera> Create(Ref<Window>& window)
	{
		return Ref<Camera>(new Camera( window));
	}


public:

	glm::mat4 UpdateProjectionMatrix();

	
	
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetInverseProjectionMatrix();

};

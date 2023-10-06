#pragma once
#include <glm/mat4x4.hpp>

#include "Common.h"
class Window;

class Camera
{
private:
	Ref<Window> window;
	glm::mat4 projection;

public:

	glm::mat4 UpdateProjectionMatrix();

	Camera(Ref<Window>& window);
	
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetInverseProjectionMatrix();

};

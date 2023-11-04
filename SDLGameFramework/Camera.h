#pragma once
#include <glm/mat4x4.hpp>

#include "Common.h"
class Window;

class Camera
{
private:
	Ref<Window> window;
	glm::mat4 projection;
	glm::mat4 view;
	glm::vec2 position;
	float zoom=1;//scale
	Camera(Ref<Window>& window);


	void UpdateViewMatrix();
protected:
	friend class Renderer;
	static Ref<Camera> Create(Ref<Window>& window)
	{
		return Ref<Camera>(new Camera( window));
	}


public:

	glm::mat4 UpdateProjectionMatrix();

	// New public methods for camera movement and zoom
	void SetPosition(const glm::vec2& newPosition);
	glm::vec2 GetPosition() const;
	void SetZoom(float newZoom);
	float GetZoom() const;

	glm::mat4 GetViewMatrix() const;
	
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetInverseProjectionMatrix();

};

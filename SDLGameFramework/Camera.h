﻿#pragma once
#include <glm/mat4x4.hpp>

#include "Common.h"
class Window;

class Camera
{
private:
	Window* window;
	glm::mat4 projection;
	glm::mat4 view;
	glm::vec2 position;
	float zoom=1;//scale
	//pixel camera variables

	float pixelsPerUnit = 64.f; // This comes from your orthographic size setup





	void UpdateViewMatrix();
protected:
	friend class Renderer;
	explicit Camera(Window* window);
	static Ref<Camera> Create(Window* window)
	{
		return Ref<Camera>(new Camera( window));
	}


public:
	float GetPixelsPerUnit() const { return pixelsPerUnit; }

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

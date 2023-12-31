﻿#include "Camera.h"

#include <SDL_video.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"

static glm::mat4 viewportNDC(int width_, int height_) {
    float minZ = -1.0f;
    float maxZ = 1.0f;

    glm::mat4 m = glm::mat4(1.0f);  // identity matrix
    m = glm::translate(m, glm::vec3(float(width_) / 2.0f, float(height_) / 2.0f, minZ));
    m = glm::scale(m, glm::vec3(float(width_) / 2.0f, -float(height_) / 2.0f, maxZ - minZ));

    return m;
}
glm::mat4 Camera::UpdateProjectionMatrix()
{
    int w = window->GetWidth(), h = window->GetHeight();
    float aspectRatio = static_cast<float>(w) / static_cast<float>(h);

    float orthoHeight = static_cast<float>(h) / pixelsPerUnit;
    float orthoWidth = orthoHeight * aspectRatio;

    glm::mat4 ndc = viewportNDC(w, h);
    glm::mat4 ortho = glm::ortho(-orthoWidth / 2.0f, orthoWidth / 2.0f, -orthoHeight / 2.0f, orthoHeight / 2.0f, -1.0f, 1.0f);
    projection = ndc * ortho;

    return projection;


}

Camera::Camera(Window* window) : window(window)
{
    UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
    view = glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0.0f));
    view = glm::scale(view, glm::vec3(zoom, zoom, 1.0f)); // Apply zoom scaling

}

void Camera::SetPosition(const glm::vec2& newPosition)
{
    position = newPosition;
    UpdateViewMatrix();
}

glm::vec2 Camera::GetPosition() const
{
    return position;
}

void Camera::SetZoom(float newZoom)
{
    zoom = newZoom > 0 ? newZoom : 1.0f; // Prevent zero or negative zoom levels
    UpdateViewMatrix();
}

float Camera::GetZoom() const
{
    return zoom;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return view;
}



glm::mat4 Camera::GetProjectionMatrix()
{
    return projection;
}

glm::mat4 Camera::GetInverseProjectionMatrix()
{
    return glm::inverse(projection);
}

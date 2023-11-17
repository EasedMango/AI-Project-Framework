#include "SteeringSystem.h"

#include "AI.h"

void SteeringSystem::OnCreate(Registry& registry)
{
}

void SteeringSystem::Update(const float& deltaTime, Registry& registry)
{
	for (auto que = registry.CreateQuery().Include<AI, Body>(); const auto & entity : que.Find()) {
		auto& ai = registry.GetComponent<AI>(entity);
		const auto& steering = ai.steering;
		auto& body = registry.GetComponent<Body>(entity);
		// Apply linear acceleration
		body.accel = steering.linear;
		if (glm::length(body.accel) > body.maxAcceleration) {
			body.accel = glm::normalize(body.accel) * body.maxAcceleration;
		}

		// Apply angular acceleration
		body.angular = steering.angular;
		if (fabs(body.angular) > body.maxAngular) {
			body.angular = (body.angular > 0 ? 1 : -1) * body.maxAngular;
		}

		// Update velocity using acceleration and time
		body.vel += body.accel * deltaTime;
		if (glm::length(body.vel) > body.maxSpeed) {
			body.vel = glm::normalize(body.vel) * body.maxSpeed;
		}

		// Assuming you have a glm::vec3 position to update
		// position += body.vel * deltaTime;

		// Update orientation using rotation and time
		body.rotation += body.angular * deltaTime;
		body.orientation += body.rotation * deltaTime;
	}
}

void SteeringSystem::OnDestroy(Registry& registry)
{
}

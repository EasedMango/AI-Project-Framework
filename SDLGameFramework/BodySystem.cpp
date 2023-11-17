#include "BodySystem.h"

#include <glm/detail/func_geometric.inl>


#include "Body.h"
#include "SpatialHash.h"

#include "Transform.h"

void UpdateBody(const float& deltaTime, Body& body, Transform& transform)
{
    transform.pos= transform.pos + body.vel * deltaTime + body.accel * (0.5f * deltaTime * deltaTime);
    body.vel = body.vel + body.accel * deltaTime;
    // Update orientation
    body.orientation += body.rotation * deltaTime;
    body.rotation += body.angular * deltaTime;

    // Clip to maxspeed, if speed exceeds max
    if (glm::length(body.vel)> body.maxSpeed)
    {
        body.vel = glm::normalize(body.vel) * body.maxSpeed;
    }

    // Clip to maxrotation, if needed
    if (transform.rot > body.maxRotation) transform.rot= body.maxRotation;

    // Could introduce dampening, of velocity and/or rotation, to simulate friction
    //vel -= 0.05 * vel;
    //rotation -= 0.05 * rotation;
}


void BodySystem::OnCreate(Registry& registry)
{
}

void BodySystem::Update(const float& deltaTime, Registry& registry)
{
	auto&& spatialHash = registry.GetComponent<SpatialHash>(registry.CreateQuery().Include<SpatialHash>().Find()[0]);
    spatialHash.clear();



	for (auto que = registry.CreateQuery().Include<Transform, Body>(); const auto & entity : que.Find()) {
		auto& transform = registry.GetComponent<Transform>(entity);
		auto& movement = registry.GetComponent<Body>(entity);

        UpdateBody(deltaTime, movement, transform);
		spatialHash.insert(entity, transform);
	}
}

void BodySystem::OnDestroy(Registry& registry)
{
}

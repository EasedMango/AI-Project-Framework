#include "MovementSystem.h"

#include "Movement.h"
#include "Transform.h"
#include "ECS/Registry.h"
void MovementSystem::OnCreate(Registry& registry)
{
}

void MovementSystem::Update(const float& deltaTime, Registry& registry)
{

	for (auto que = registry.CreateQuery().Include<Transform, Movement>(); const auto & entity : que.Find()) {
		auto& transform = registry.GetComponent<Transform>(entity);
		auto& movement = registry.GetComponent<Movement>(entity);

		transform.position += glm::vec3(movement.dir ,0.f)* movement.speed * deltaTime;
	}
}

void MovementSystem::OnDestroy(Registry& registry)
{
}

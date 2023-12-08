#include "CollisionSystem.h"

#include "Body.h"
#include "Collider.h"
#include "SpatialHash.h"
#include <array>





void CircleCircleCollision(Registry& registry, ID entity1, ID entity2, float deltaTime) {
	auto& transform1 = registry.GetComponent<Transform>(entity1);
	auto& transform2 = registry.GetComponent<Transform>(entity2);

	const auto& circleCollider1 = registry.GetComponent<CircleCollider>(entity1);
	const auto& circleCollider2 = registry.GetComponent<CircleCollider>(entity2);

	const CollisionResult result = CheckCircleCircleCollision(circleCollider1, transform1, circleCollider2, transform2);
	if (!result.isColliding) {
		return;
	}
	const auto& collider1 = registry.GetComponent<Collider>(entity1);
	const auto& collider2 = registry.GetComponent<Collider>(entity2);
	// Apply a little extra separation to avoid sticking due to floating-point precision issues.
	constexpr float separationFactor = 1.01f;
	if (collider1.dynamic) {
		constexpr float movePercentage = 0.5f;
		const glm::vec2 moveAmount = result.mtv * movePercentage;
		transform1.pos += glm::vec3(moveAmount * separationFactor, 0.f);
	}
	if (collider2.dynamic) {
		constexpr float movePercentage = 0.5f;
		const glm::vec2 moveAmount = result.mtv * movePercentage;
		transform2.pos -= glm::vec3(moveAmount * separationFactor, 0.f);

	}
}



void BoxBoxCollision(Registry& registry, ID entity1, ID entity2, const float& deltaTime)
{
	auto& transform1 = registry.GetComponent<Transform>(entity1);
	auto& transform2 = registry.GetComponent<Transform>(entity2);

	const auto& boxCollider1 = registry.GetComponent<BoxCollider>(entity1);
	const auto& boxCollider2 = registry.GetComponent<BoxCollider>(entity2);

	const auto& collider1 = registry.GetComponent<Collider>(entity1);
	const auto& collider2 = registry.GetComponent<Collider>(entity2);

	const auto corners1 = calculateOBBCorners(transform1, boxCollider1);
	const auto corners2 = calculateOBBCorners(transform2, boxCollider2);

	const auto result = CheckOBBOBBCollision(corners1, corners2);
	if (!result.isColliding) {
		return;
	}




	// Apply a little extra separation to avoid sticking due to floating-point precision issues.
	constexpr float separationFactor = 1.01f;
	if (collider1.dynamic) {
		constexpr float movePercentage = 0.5f;
		const glm::vec2 moveAmount = result.mtv * movePercentage;
		transform1.pos -= glm::vec3(moveAmount * separationFactor, 0.f);
	}
	if (collider2.dynamic) {
		constexpr float movePercentage = 0.5f;
		const glm::vec2 moveAmount = result.mtv * movePercentage;
		transform2.pos += glm::vec3(moveAmount * separationFactor, 0.f);

	}
}

void CircleOBBCollision(Registry& registry, ID entity1, ID entity2, const float& deltaTime)
{

	auto& transform1 = registry.GetComponent<Transform>(entity1); // Circle transform
	auto& transform2 = registry.GetComponent<Transform>(entity2); // OBB transform

	const auto& circleCollider = registry.GetComponent<CircleCollider>(entity1);
	const auto& boxCollider = registry.GetComponent<BoxCollider>(entity2);

	const CollisionResult result = CheckCircleOBBCollision(circleCollider, transform1, boxCollider, transform2);

	if (!result.isColliding) {
		return;
	}
	const auto& collider1 = registry.GetComponent<Collider>(entity1);
	const auto& collider2 = registry.GetComponent<Collider>(entity2);
	// Apply a little extra separation to avoid sticking due to floating-point precision issues.
	constexpr float separationFactor = 1.01f;
	if (collider1.dynamic) {
		constexpr float movePercentage = 0.5f;
		const glm::vec2 moveAmount = result.mtv * movePercentage;
		transform1.pos += glm::vec3(moveAmount * separationFactor, 0.f);
	}
	if (collider2.dynamic) {
		constexpr float movePercentage = 0.5f;
		const glm::vec2 moveAmount = result.mtv * movePercentage;
		transform2.pos -= glm::vec3(moveAmount * separationFactor, 0.f);

	}
}

void CollisionSystem::CheckCollision(Registry& registry, const ID& entity1, const ID& entity2, const float& deltaTime)
{

	static auto&& spatialHash = registry.GetComponent<SpatialHash>(registry.CreateQuery().Include<SpatialHash>().Find()[0]);

	const auto& collider1 = registry.GetComponent<Collider>(entity1);
	const auto& collider2 = registry.GetComponent<Collider>(entity2);

	switch (collider1.shape)
	{
	case ColliderShape::BoxCollider:
		switch (collider2.shape)
		{
		case ColliderShape::BoxCollider:
			BoxBoxCollision(registry, entity1, entity2, deltaTime);
			break;
		case ColliderShape::CircleCollider:
			CircleOBBCollision(registry, entity2, entity1, deltaTime);
			break;
		default:
			break;
		}
		break;


	case ColliderShape::CircleCollider:
		switch (collider2.shape)
		{
		case ColliderShape::BoxCollider:
			CircleOBBCollision(registry, entity1, entity2, deltaTime);
			break;
		case ColliderShape::CircleCollider:
			CircleCircleCollision(registry, entity1, entity2, deltaTime);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}



}

void CollisionSystem::OnCreate(Registry& registry)
{
}

void CollisionSystem::Update(const float& deltaTime, Registry& registry)
{


	auto&& spatialHash = registry.GetComponent<SpatialHash>(registry.CreateQuery().Include<SpatialHash>().Find()[0]);
	for (auto que = registry.CreateQuery().Include<Collider>(); const auto & entity : que.Find()) {

		auto& transform = registry.GetComponent<Transform>(entity);
		//auto& state = registry.GetComponent<CollisionState>(entity);

		//state.collisionEvents.clear();

		auto cell = spatialHash.query(transform);

		for (auto& otherEntity : cell)
		{
			if (entity == otherEntity)
				continue;

			CheckCollision(registry, entity, otherEntity, deltaTime);
		}
	}
}

void CollisionSystem::OnDestroy(Registry& registry)
{
}

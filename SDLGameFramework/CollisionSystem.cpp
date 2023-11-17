#include "CollisionSystem.h"

#include "Body.h"
#include "Collider.h"
#include "Transform.h"
#include <glm/mat2x2.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/norm.hpp> 
#include "SpatialHash.h"

#include <array>
auto calculateOBBCorners = [](const Transform& transform, const BoxCollider& collider) {
	const auto rotationMatrix =
		glm::mat2x2(glm::cos(collider.rotation + transform.rot), glm::sin(collider.rotation + transform.rot),
			-glm::sin(collider.rotation + transform.rot), glm::cos(collider.rotation + transform.rot));


	std::array<glm::vec2, 4> vertices = {
		glm::vec2(transform.pos) + collider.offset + rotationMatrix * glm::vec2(collider.halfWidth, collider.halfHeight)  ,
		glm::vec2(transform.pos) + collider.offset + rotationMatrix * glm::vec2(-collider.halfWidth, collider.halfHeight),
		glm::vec2(transform.pos) + collider.offset + rotationMatrix * glm::vec2(-collider.halfWidth, -collider.halfHeight),
		glm::vec2(transform.pos) + collider.offset + rotationMatrix * glm::vec2(collider.halfWidth, -collider.halfHeight)
	};

	return vertices;
	};
auto ProjectOntoAxis(const std::array<glm::vec2, 4>& corners, const glm::vec2& axis) {
	float min = std::numeric_limits<float>::max();
	float max = std::numeric_limits<float>::min();
	for (const auto& corner : corners) {
		const float projection = glm::dot(corner, axis);
		min = std::min(min, projection);
		max = std::max(max, projection);
	}
	return std::make_pair(min, max);
}


bool CheckOverlapOnAxis(const std::array<glm::vec2, 4>& corners1, const std::array<glm::vec2, 4>& corners2, const glm::vec2& axis) {
	const auto [min1, max1] = ProjectOntoAxis(corners1, axis);
	const auto [min2, max2] = ProjectOntoAxis(corners2, axis);
	return (min2 <= max1) && (min1 <= max2);
}

bool CheckOverlapOnAxis(const std::array<glm::vec2, 4>& corners1, const std::array<glm::vec2, 4>& corners2, const glm::vec2& axis, float& overlapOut) {
	const auto [min1, max1] = ProjectOntoAxis(corners1, axis);
	const auto [min2, max2] = ProjectOntoAxis(corners2, axis);
	bool isOverlapping = (min2 <= max1) && (min1 <= max2);
	if (isOverlapping) {
		float overlap1 = max1 - min2;
		float overlap2 = max2 - min1;
		overlapOut = std::min(overlap1, overlap2);
	}
	return isOverlapping;
}

glm::vec2 CalculateMTV(const glm::vec2& axis, const float& overlap, const glm::vec2& relativePosition) {
	glm::vec2 mtv = axis * overlap;
	// Ensure the MTV points from OBB2 to OBB1
	if (glm::dot(mtv, relativePosition) < 0) {
		mtv = -mtv;
	}
	return mtv;
}
struct SATResult {
	bool isColliding;         // Flag indicating if a collision occurred
	glm::vec2 collisionNormal; // The normal of the collision
	float overlap;            // The amount of overlap between the two OBBs
	glm::vec2 mtv;            // The Minimum Translation Vector to resolve the collision

	// Constructor for ease of use
	SATResult(bool isColliding, glm::vec2 collisionNormal, float overlap, glm::vec2 mtv)
		: isColliding(isColliding), collisionNormal(collisionNormal), overlap(overlap), mtv(mtv) {}

	// Default constructor for no collision
	SATResult() : isColliding(false), collisionNormal(), overlap(0.0f), mtv()
	{
	}
};


SATResult CheckSATOBBCollision(const std::array<glm::vec2, 4>& corners1, const std::array<glm::vec2, 4>& corners2)
{
	SATResult result;
	result.overlap = std::numeric_limits<float>::infinity();
	std::array<glm::vec2, 4> axes = {
		glm::normalize(corners1[0] - corners1[1]),
		glm::normalize(corners1[0] - corners1[3]),
		glm::normalize(corners2[1] - corners2[0]),
		glm::normalize(corners2[3] - corners2[0])
	};

	const glm::vec2 relativePosition = corners2[0] - corners1[0]; // Vector from OBB1 to OBB2
	for (auto& axis : axes) {
		float length = glm::length(axis);
		if (length > std::numeric_limits<float>::epsilon()) {
			axis = glm::normalize(axis);
		}
		else {
			continue;
		}
		float overlap;
		if (!CheckOverlapOnAxis(corners1, corners2, axis, overlap)) {
			return SATResult(); // No collision
		}
		if (overlap < result.overlap) {
			result.isColliding = true;
			result.overlap = overlap;
			result.collisionNormal = axis;
			result.mtv = CalculateMTV(axis, overlap, relativePosition);
		}
	}

	return result;
}






void BoxBoxCollision(Registry& registry, ID entity1, ID entity2, const float& deltaTime)
{
	auto& transform1 = registry.GetComponent<Transform>(entity1);
	auto& transform2 = registry.GetComponent<Transform>(entity2);

	auto& boxCollider1 = registry.GetComponent<BoxCollider>(entity1);
	auto& boxCollider2 = registry.GetComponent<BoxCollider>(entity2);

	auto& rigidBody1 = registry.GetComponent<Body>(entity1);
	auto& rigidBody2 = registry.GetComponent<Body>(entity2);

	auto corners1 = calculateOBBCorners(transform1, boxCollider1);
	auto corners2 = calculateOBBCorners(transform2, boxCollider2);

	auto result = CheckSATOBBCollision(corners1, corners2);
	if (!result.isColliding) {
		return;
	}

	float inverseMass1 = rigidBody1.mass > 0 ? 1 / rigidBody1.mass : 0;
	float inverseMass2 = rigidBody2.mass > 0 ? 1 / rigidBody2.mass : 0;
	float totalInverseMass = inverseMass1 + inverseMass2;

	if (totalInverseMass <= 0) return;

	float movePercentage1 = inverseMass1 / totalInverseMass;
	float movePercentage2 = inverseMass2 / totalInverseMass;

	glm::vec2 moveAmount1 = result.mtv * movePercentage1;
	glm::vec2 moveAmount2 = result.mtv * movePercentage2;

	// Apply a little extra separation to avoid sticking due to floating-point precision issues.
	const float separationFactor = 1.01f;
	transform1.pos -= glm::vec3(moveAmount1 * separationFactor, 0.f);
	transform2.pos += glm::vec3(moveAmount2 * separationFactor, 0.f);


}

void CollisionSystem::CheckCollision(Registry& registry, const ID& entity1, const ID& entity2, const float& deltaTime)
{

	static auto&& spatialHash = registry.GetComponent<SpatialHash>(registry.CreateQuery().Include<SpatialHash>().Find()[0]);

	auto& collider1 = registry.GetComponent<Collider>(entity1);
	auto& collider2 = registry.GetComponent<Collider>(entity2);


	BoxBoxCollision(registry, entity1, entity2, deltaTime);





	/*switch (collider1.shape)
	{
	case ColliderShape::BoxCollider:
		switch (collider2.shape)
		{
		case ColliderShape::BoxCollider:
			BoxBoxCollision(registry, entity1, entity2, deltaTime);
			break;
		case ColliderShape::CircleCollider:
			break;
		case ColliderShape::CustomCollider:
			break;
		default:
			break;
		}
		break;


	case ColliderShape::CircleCollider:
		switch (collider2.shape)
		{
		case ColliderShape::BoxCollider:
			break;
		case ColliderShape::CircleCollider:
			break;
		case ColliderShape::CustomCollider:
			break;
		default:
			break;
		}
		break;

	case ColliderShape::CustomCollider:
		switch (collider2.shape)
		{
		case ColliderShape::BoxCollider:
			break;
		case ColliderShape::CircleCollider:
			break;
		case ColliderShape::CustomCollider:
			break;
		default:
			break;
		}
		break;
	default:;
	}*/



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

#include "CollisionSystem.h"

#include "Body.h"
#include "Collider.h"
#include "Transform.h"
#include <glm/mat2x2.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/norm.hpp> 
#include "SpatialHash.h"
#include <array>


struct CollisionResult {
	bool isColliding;         // Flag indicating if a collision occurred
	glm::vec2 collisionNormal; // The normal of the collision
	float overlap;            // The amount of overlap between the two OBBs
	glm::vec2 mtv;            // The Minimum Translation Vector to resolve the collision

	// Constructor for ease of use
	CollisionResult(bool isColliding, glm::vec2 collisionNormal, float overlap, glm::vec2 mtv)
		: isColliding(isColliding), collisionNormal(collisionNormal), overlap(overlap), mtv(mtv) {}

	// Default constructor for no collision
	CollisionResult() : isColliding(false), collisionNormal(), overlap(0.0f), mtv()
	{
	}
};

inline std::array<glm::vec2, 4> calculateOBBCorners(const Transform& transform, const BoxCollider& collider)
{
	const auto rot = glm::radians(transform.rot + collider.rotation);
	const auto pos = glm::vec2(transform.pos) + collider.offset;
	const auto rotCos = glm::cos(rot);
	const auto rotSin = glm::sin(rot);
	/*if (rot >= FLT_EPSILON && rot <=)*/
	const auto rotationMatrix =
		glm::mat2x2(rotCos, rotSin,
			-rotSin, rotCos);


	const std::array<glm::vec2, 4> vertices = {
		pos + rotationMatrix * glm::vec2(collider.halfWidth, collider.halfHeight)  ,
		pos + rotationMatrix * glm::vec2(-collider.halfWidth, collider.halfHeight),
		pos + rotationMatrix * glm::vec2(-collider.halfWidth, -collider.halfHeight),
		pos + rotationMatrix * glm::vec2(collider.halfWidth, -collider.halfHeight)
	};

	return vertices;
};

inline std::pair<float, float> ProjectOntoAxis(const std::array<glm::vec2, 4>& corners, const glm::vec2& axis)
{
	float min = std::numeric_limits<float>::max();
	float max = std::numeric_limits<float>::min();
	for (const auto& corner : corners) {
		const float projection = glm::dot(corner, axis);
		min = std::min(min, projection);
		max = std::max(max, projection);
	}
	return std::make_pair(min, max);
};

inline bool CheckOverlapOnAxis(const std::array<glm::vec2, 4>& corners1, const std::array<glm::vec2, 4>& corners2, const glm::vec2& axis, float& overlapOut)
{
	const auto [min1, max1] = ProjectOntoAxis(corners1, axis);
	const auto [min2, max2] = ProjectOntoAxis(corners2, axis);
	const bool isOverlapping = (min2 <= max1) && (min1 <= max2);
	if (isOverlapping) {
		const float overlap1 = max1 - min2;
		const float overlap2 = max2 - min1;
		overlapOut = std::min(overlap1, overlap2);
	}
	return isOverlapping;
};
inline glm::vec2 CalculateMTV(const glm::vec2& axis, float overlap, const glm::vec2& relativePosition)
{
	glm::vec2 mtv = axis * overlap;
	// Ensure the MTV points from OBB2 to OBB1
	if (glm::dot(mtv, relativePosition) < 0) {
		mtv = -mtv;
	}
	return mtv;
};

inline CollisionResult CheckOBBOBBCollision(const std::array<glm::vec2, 4>& corners1, const std::array<glm::vec2, 4>& corners2)
{
	CollisionResult result;
	result.overlap = std::numeric_limits<float>::infinity();
	std::array<glm::vec2, 4> axes = {
		glm::normalize(corners1[0] - corners1[1]),
		glm::normalize(corners1[0] - corners1[3]),
		glm::normalize(corners2[1] - corners2[0]),
		glm::normalize(corners2[3] - corners2[0])
	};

	const glm::vec2 relativePosition = corners2[0] - corners1[0]; // Vector from OBB1 to OBB2
	for (auto& axis : axes) {
		const float length = glm::length(axis);
		if (length > std::numeric_limits<float>::epsilon()) {
			axis = glm::normalize(axis);
		}
		else {
			continue;
		}
		float overlap;
		if (!CheckOverlapOnAxis(corners1, corners2, axis, overlap)) {
			return {}; // No collision
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
CollisionResult CheckCircleCircleCollision(const CircleCollider& circle1, const Transform& transform1, const CircleCollider& circle2, const Transform& transform2) {
	const glm::vec2 distanceVec = (glm::vec2(transform1.pos) + circle2.offset) - (glm::vec2(transform2.pos) + circle1.offset);
	const float distance = glm::length(distanceVec);
	const float radiusSum = circle1.radius + circle2.radius;

	if (distance < radiusSum) {
		float overlap = radiusSum - distance;
		glm::vec2 collisionNormal = glm::normalize(distanceVec);
		glm::vec2 mtv = collisionNormal * overlap;
		return { true, collisionNormal, overlap, mtv };
	}

	return {}; // No collision
}
CollisionResult CheckCircleOBBCollision(const CircleCollider& circle1, const Transform& transform1, const BoxCollider& boxCollider, const Transform& transform2) {
	// Calculate world space position of the circle center
	const glm::vec2 circleCenter = glm::vec2(transform1.pos) + circle1.offset;

	// Transform the circle center to the OBB's local space
	const glm::mat2 rotationMatrix = glm::mat2(
		glm::cos(glm::radians(-transform2.rot)), glm::sin(glm::radians(-transform2.rot)),
		-glm::sin(glm::radians(-transform2.rot)), glm::cos(glm::radians(-transform2.rot))
	);
	const glm::vec2 localCircleCenter = rotationMatrix * (circleCenter - glm::vec2(transform2.pos));


	const glm::vec2 halfExtents = { boxCollider.halfWidth, boxCollider.halfHeight };
	// Find the closest point on the OBB to the local circle center
	const glm::vec2 closestPoint = glm::clamp(localCircleCenter, -halfExtents, halfExtents);

	// Transform the closest point back to world space
	const glm::vec2 worldClosestPoint = glm::inverse(rotationMatrix) * closestPoint + glm::vec2(transform2.pos);

	// Calculate the vector from the circle's center to the closest point and its magnitude
	const glm::vec2 toCircle = circleCenter - worldClosestPoint;
	const float distance = glm::length(toCircle);

	// Check for collision
	if (distance < circle1.radius) {
		const float overlap = circle1.radius - distance;
		const glm::vec2 collisionNormal = glm::normalize(toCircle);
		const glm::vec2 mtv = collisionNormal * overlap;
		return CollisionResult(true, collisionNormal, overlap, mtv);
	}

	return CollisionResult(); // No collision
}


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

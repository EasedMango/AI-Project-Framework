#pragma once
#include <array>
#include<glm/glm.hpp>

#include "Transform.h"


enum class ColliderShape
{
	BoxCollider,
	CircleCollider,
	CustomCollider
};
enum class State {
	None,
	Enter,
	Stay,
	Exit
};

struct CollisionEvent
{
	bool isColliding;
	ID entity;
	glm::vec2 contactPoint;
	glm::vec2 contactNormal;
	float penetration;
	State state;
};

struct CollisionState
{
	bool isColliding;
	std::unordered_map<ID, CollisionEvent> collisionEvents;
	CollisionState() : isColliding(false), collisionEvents(std::unordered_map<ID, CollisionEvent>())
	{
		collisionEvents.reserve(5);
	}
};


struct Collider
{
	ColliderShape shape;
	unsigned int layer;
	bool dynamic = true;
	//CollisionState collisionState;
};

struct BoxCollider
{

	float halfWidth;
	float halfHeight;
	float rotation;
	glm::vec2 offset;
};

struct CircleCollider
{
	glm::vec2 offset;
	float radius;
};

struct CollisionResult {
	bool isColliding;         // Flag indicating if a collision occurred
	glm::vec2 collisionNormal; // The normal of the collision
	float overlap;            // The amount of overlap between the two OBBs
	glm::vec2 mtv;            // The Minimum Translation Vector to resolve the collision

	bool operator()() const
	{
		return isColliding;
	}

	// Constructor for ease of use
	CollisionResult(bool isColliding, glm::vec2 collisionNormal, float overlap, glm::vec2 mtv)
		: isColliding(isColliding), collisionNormal(collisionNormal), overlap(overlap), mtv(mtv) {}

	// Default constructor for no collision
	CollisionResult() : isColliding(false), collisionNormal(), overlap(0.0f), mtv()
	{
	}

	CollisionResult& operator=(const bool isColliding)
	{
		this->isColliding = isColliding;
		return *this;
	}
};

struct Ray
{
	glm::vec2 origin;
	glm::vec2 direction;
	float distance = FLT_MAX;
};

inline CollisionResult RayIntersectsOBB(const Ray& ray, const glm::vec2& obbCenter, const glm::vec2& obbHalfExtents, const glm::mat2x2& obbRotation) {
	// Transform ray to OBB's local space
	CollisionResult cr;
	glm::mat2x2 invRotation = glm::inverse(obbRotation);
	const glm::vec2 localOrigin = invRotation * (ray.origin - obbCenter);
	const glm::vec2 localDirection = invRotation * ray.direction;

	const glm::vec2 aabbMin = -obbHalfExtents;
	const glm::vec2 aabbMax = obbHalfExtents;

	const glm::vec2 tValuesMin = (aabbMin - localOrigin) / localDirection;
	const glm::vec2 tValuesMax = (aabbMax - localOrigin) / localDirection;

	const glm::vec2 tMinVec = glm::min(tValuesMin, tValuesMax);
	const glm::vec2 tMaxVec = glm::max(tValuesMin, tValuesMax);

	cr.overlap = glm::max(tMinVec.x, tMinVec.y);
	const float tMax = glm::min(tMaxVec.x, tMaxVec.y);



	if (cr.overlap > tMax || tMax < 0.0f) {
		return cr = false; // No intersection
	}

	// Determine which face is hit and set the normal
    if (cr.overlap == tMinVec.x) {
        cr.collisionNormal = localDirection.x < 0 ? glm::vec2(1, 0) : glm::vec2(-1, 0);
    } else {
        cr.collisionNormal = localDirection.y < 0 ? glm::vec2(0, 1) : glm::vec2(0, -1);
    }

	return cr = true;
}

inline CollisionResult RayIntersectsCircle(const Ray& ray, const glm::vec2& circleCenter, float circleRadius) {
	CollisionResult cr;
	const glm::vec2 toCircle = circleCenter - ray.origin;
	const float tClosest = glm::dot(toCircle, ray.direction);
	const glm::vec2 closestPoint = ray.origin + tClosest * ray.direction;
	const glm::vec2 circleToRay = closestPoint - circleCenter;

	const float distanceSquared = glm::dot(circleToRay, circleToRay);
	const float radiusSquared = circleRadius * circleRadius;

	if (distanceSquared > radiusSquared) {
		return cr = false; // No intersection
	}

	const float tOffset = glm::sqrt(radiusSquared - distanceSquared);

	cr.overlap = tClosest - tOffset;

	if (cr.overlap < 0.0f) {
		cr.overlap = tClosest + tOffset;
	}
	//get normal
	auto normal = glm::normalize(closestPoint - circleCenter);
	cr.collisionNormal = normal;

	if (cr.overlap >= 0.0f)
		return cr = true;

	return cr = false; // No intersection

}

inline

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

inline CollisionResult CheckCircleCircleCollision(const CircleCollider& circle1, const Transform& transform1, const CircleCollider& circle2, const Transform& transform2) {
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

inline CollisionResult CheckCircleOBBCollision(const CircleCollider& circle1, const Transform& transform1, const BoxCollider& boxCollider, const Transform& transform2) {
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
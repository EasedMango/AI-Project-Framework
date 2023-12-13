#pragma once
#include <array>
#include <unordered_map>
#include<glm/glm.hpp>

#include "Common.h"


struct Transform;



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
	glm::vec2 collisionPoint;


	bool Valid() const
	{
		bool valid = glm::any(glm::isnan(collisionNormal)) == false && glm::any(glm::isnan(mtv)) == false && glm::any(glm::isnan(collisionPoint)) == false;
		return valid;
	}

	bool operator()() const
	{
		return isColliding;
	}

	// Constructor for ease of use
	CollisionResult(bool isColliding, glm::vec2 collisionNormal, float overlap, glm::vec2 mtv)
		: isColliding(isColliding), collisionNormal(collisionNormal), overlap(overlap), mtv(mtv), collisionPoint()
	{
	}

	// Default constructor for no collision
	CollisionResult() : isColliding(false), collisionNormal(), overlap(0.0f), mtv(), collisionPoint()
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

 CollisionResult RayIntersectsOBB(const Ray& ray, const glm::vec2& obbCenter, const glm::vec2& obbHalfExtents, const glm::mat2x2& obbRotation);

 CollisionResult RayIntersectsCircle(const Ray& ray, const glm::vec2& circleCenter, float circleRadius);


 std::array<glm::vec2, 4> calculateOBBCorners(const Transform& transform, const BoxCollider& collider);;

 std::pair<float, float> ProjectOntoAxis(const std::array<glm::vec2, 4>& corners, const glm::vec2& axis);;

 bool CheckOverlapOnAxis(const std::array<glm::vec2, 4>& corners1, const std::array<glm::vec2, 4>& corners2, const glm::vec2& axis, float& overlapOut);

 glm::vec2 CalculateMTV(const glm::vec2& axis, float overlap, const glm::vec2& relativePosition);;

 CollisionResult CheckOBBOBBCollision(const std::array<glm::vec2, 4>& corners1, const std::array<glm::vec2, 4>& corners2);

 CollisionResult CheckCircleCircleCollision(const CircleCollider& circle1, const Transform& transform1, const CircleCollider& circle2, const Transform& transform2);

 CollisionResult CheckCircleOBBCollision(const CircleCollider& circle1, const Transform& transform1, const BoxCollider& boxCollider, const Transform& transform2);

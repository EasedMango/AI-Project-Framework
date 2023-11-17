#pragma once
#include <glm/vec2.hpp>

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

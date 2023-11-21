#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "AIBehaviors.h"
#include "Common.h"
#include "Grid.h"

struct SeekInfo
{
	ID target;
	float maxAcceleration;
};

struct ArriveInfo
{
	ID target;
	float maxSpeed;
	float maxAcceleration;
	float targetRadius;
	float slowRadius;
	float timeToTarget;
};

struct ChaseInfo
{
	ID target;
};

struct FleeInfo
{
	ID target;
	float maxAcceleration;
};


struct WanderInfo
{
	float range;
	glm::vec2 center;
	float speed;
	std::vector<Tile*> path;
	int currentPathIndex;

};

struct PatrolInfo
{
	ID target;
	glm::vec3 patrolPointA;
	glm::vec3 patrolPointB;
	bool AtpointA;
};

struct WeightedBehavior
{
	AIBehaviors::BehaviorType behavior;
	float weight;
};


struct AI
{
	std::vector<WeightedBehavior> behaviors;
	SteeringOutput steering;


};




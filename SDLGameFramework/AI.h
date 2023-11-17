#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "AIBehaviors.h"
#include "Common.h"
#include "Grid.h"

struct SeekInfo
{
	float maxAcceleration;
};

struct ArriveInfo
{
	float maxSpeed;
	float maxAcceleration;
	float targetRadius;
	float slowRadius;
	float timeToTarget;
};

struct ChaseInfo
{

};

struct FleeInfo
{

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
	glm::vec3 patrolPointA;
	glm::vec3 patrolPointB;
	bool AtpointA;
};




struct AI
{
	AIBehaviors::BehaviorType currentBehavior;
	ID target;

	SteeringOutput steering;

};



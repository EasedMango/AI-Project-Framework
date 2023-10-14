#pragma once
#include <glm/vec3.hpp>

#include "AIBehaviors.h"
#include "Common.h"

struct SeekInfo
{
	float maxAcceleration;
};

struct ArriveInfo
{

};

struct ChaseInfo
{

};

struct FleeInfo
{

};

struct WanderInfo
{

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



#pragma once
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
struct AI
{
	AIBehaviors::BehaviorType currentBehavior;
	ID target;

	SteeringOutput steering;

};



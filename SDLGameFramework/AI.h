#pragma once
#include "AIBehaviors.h"
#include "Common.h"

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
struct AI
{
	AIBehaviors::BehaviorType currentBehavior;
	ID target;

	SeekInfo seek;
	ArriveInfo arrive;



	
	//AI() : currentBehavior(AIBehaviors::BehaviorType::Seek), target(-1), currentBehaviorInfo(SeekInfo{ 0.0f }) {}

	//AI(AIBehaviors::BehaviorType type, behaviorInfo info) : currentBehavior(type), target(-1), currentBehaviorInfo(info)
	//{
	//}

	//AI(AIBehaviors::BehaviorType type, SeekInfo info) : currentBehavior(type), target(-1), currentBehaviorInfo(info)
	//{
	//}

	//AI(AIBehaviors::BehaviorType type, ArriveInfo info) : currentBehavior(type), target(-1), currentBehaviorInfo(info)
	//{
	//}

};



#pragma once
#include <glm/geometric.hpp>


#include "Body.h"
#include "Grid.h"
#include "SteeringOutput.h"
#include "Transform.h"


struct WanderInfo;
struct PatrolInfo;
struct ArriveInfo;
struct SeekInfo;
struct ChaseInfo;
struct FleeInfo;
namespace AIBehaviors {

	enum class BehaviorType
	{
		Seek,
		Chase,
		Arrive,
		Patrol,
		Wander,
		Flee
	};


	SteeringOutput Seek(const Body& characterBody, const Transform& characterTrans, const Transform& target, SeekInfo& info);

	SteeringOutput Chase(Body& characterBody, const Transform& characterTrans, const Transform& target, ChaseInfo& info);

	SteeringOutput Arrive(const Body& characterBody, const Transform& characterTrans, const Transform& target, ArriveInfo& info);
	
	SteeringOutput Patrol(const Body& characterBody, const Transform& characterTrans, const Transform& target, PatrolInfo& info);

	SteeringOutput Wander(const Body& characterBody, const Transform& characterTrans, Grid& target, WanderInfo& info);


	SteeringOutput Flee(Body& characterBody, const Transform& characterTrans, const Transform& target, FleeInfo& info);

}

#pragma once
#include <glm/geometric.hpp>


#include "Body.h"
#include "SteeringOutput.h"
#include "Transform.h"


struct PatrolInfo;
struct ArriveInfo;
struct SeekInfo;
struct ChaseInfo;

namespace AIBehaviors {

	enum class BehaviorType
	{
		Seek,
		Chase,
		Arrive,
		Patrol
	};


	SteeringOutput Seek(const Body& characterBody, const Transform& characterTrans, const Transform& target, SeekInfo& info);

	SteeringOutput Chase(Body& characterBody, const Transform& characterTrans, const Transform& target, ChaseInfo& info);

	SteeringOutput Arrive(const Body& characterBody, const Transform& characterTrans, const Transform& target, ArriveInfo& info);
	
	SteeringOutput Patrol(const Body& characterBody, const Transform& characterTrans, const Transform& target, PatrolInfo& info);

}

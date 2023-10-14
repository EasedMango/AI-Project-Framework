
#include "AIBehaviors.h"
#include "SteeringOutput.h"
#include "AI.h"


SteeringOutput AIBehaviors::Seek(const Body& characterBody, const Transform& characterTrans, const Transform& target, SeekInfo& info)
{
	SteeringOutput steering;
	steering.angular = 0;

	glm::vec3 direction = target.pos - characterTrans.pos;
	float distance = glm::length(direction);

	if (distance > 0) {
		float targetSpeed;

		// If the target is close enough, then slow down


		targetSpeed = characterBody.maxSpeed;


		glm::vec3 targetVelocity = glm::normalize(direction) * targetSpeed;

		// Accelerate towards the target
		steering.linear = targetVelocity - characterBody.vel;

		// Optionally limit the acceleration
		if (glm::length(steering.linear) > characterBody.maxAcceleration) {
			steering.linear = glm::normalize(steering.linear) * characterBody.maxAcceleration;
		}
	}
	else {
		steering.linear = glm::vec3(0.0f);
	}

	return steering;
}
SteeringOutput AIBehaviors::Arrive(const Body& characterBody, const Transform& characterTrans, const Transform& target, ArriveInfo& info)
{
	return SteeringOutput();
}

SteeringOutput AIBehaviors::Patrol(const Body& characterBody, const Transform& characterTrans, const Transform& target, PatrolInfo& info)
{
	SteeringOutput steering;
	steering.angular = 0;

	glm::vec3 direction = target.pos - characterTrans.pos;
	float distance = glm::length(direction);
	
	
	SeekInfo si = SeekInfo(1.0f);
	
	if (distance < 2) {

		return Seek(characterBody,characterTrans,target,si);
	}


	//sets patrol point
	info.patrolPointA = glm::vec3(5.0f, 5.0f, 0.0f);
	info.patrolPointB = glm::vec3(-5.0f, -5.0f, 0.0f);

	//sets the starting posistion to be one of the points
	glm::vec3 currentPosition;

	if (info.AtpointA == true) {
		steering.linear = characterTrans.pos - info.patrolPointB;
		currentPosition = info.patrolPointB;
	}

	if (info.AtpointA == false) {
		steering.linear = characterTrans.pos - info.patrolPointA;
		currentPosition = info.patrolPointA;
	}
	
	 direction = currentPosition - characterTrans.pos;
	 distance = glm::length(direction);
	
	 if (distance <= 1.0f) {
		 info.AtpointA = !info.AtpointA;
	 }

	return steering;
}
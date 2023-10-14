
#include "AIBehaviors.h"
#include "SteeringOutput.h"
#include "AI.h"


SteeringOutput AIBehaviors::Seek(Body& characterBody, const Transform& characterTrans, const Transform& target, SeekInfo& info)
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

SteeringOutput AIBehaviors::Chase(Body& characterBody, const Transform& characterTrans, const Transform& target, ChaseInfo& info) 
{
	SteeringOutput steering;
	steering.angular = 0;

	glm::vec3 direction = target.pos - characterTrans.pos;
	float distance = glm::length(direction);

	// Work out current speed
	float targetSpeed = glm::length(characterBody.vel);

	// Decide on a reasonable prediction time
	float maxPrediction = 1.0f;
	float prediction;

	if (targetSpeed <= (distance / maxPrediction))
	{
		prediction = maxPrediction;
	}
	else
	{
		prediction = distance / targetSpeed;
	}
	 
	//float targetSpeed;

	// If the target is close enough, then slow down

	targetSpeed = characterBody.maxSpeed;
	glm::vec3 targetVelocity = glm::normalize(direction) * targetSpeed;

	// Put together the target for Chase
	Transform ChaseTarget = target;
	ChaseTarget.pos += targetVelocity * prediction; 

	// Call the Seek function with the new target; 

	SeekInfo si = SeekInfo(1.0f);

	return Seek(characterBody, characterTrans, ChaseTarget, si);

}

SteeringOutput AIBehaviors::Arrive(const Body& characterBody, const Transform& characterTrans, const Transform& target, ArriveInfo& info)
{
	return SteeringOutput();
}
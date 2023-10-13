
#include "AIBehaviors.h"

#include "AI.h"


SteeringOutput AIBehaviors::Seek(Body& characterBody, const Transform& characterTrans, const Transform& target, SeekInfo& info)
{
	SteeringOutput steering;
	steering.linear = target.pos - characterTrans.pos;
	steering.linear = glm::normalize(steering.linear);
	steering.linear *= info.maxAcceleration;
	steering.angular = 0;
	return steering;
}
SteeringOutput AIBehaviors::Arrive(const Body& characterBody, const Transform& characterTrans, const Transform& target, ArriveInfo& info)
{
	SteeringOutput steering;
	glm::vec3 direction = target.pos - characterTrans.pos;
	float distance = glm::length(direction);
	if (distance < info.targetRadius)
	{
		steering.linear = glm::vec3(0, 0, 0);
		steering.angular = 0;
		return steering;
	}
	float targetSpeed;
	if (distance > info.slowRadius)
	{
		targetSpeed = info.maxSpeed;
	}
	else
	{
		targetSpeed = info.maxSpeed * distance / info.slowRadius;
	}
	glm::vec3 targetVelocity = direction;
	targetVelocity = glm::normalize(targetVelocity);
	targetVelocity *= targetSpeed;
	steering.linear = targetVelocity - characterBody.vel;
	steering.linear /= info.timeToTarget;
	if (glm::length(steering.linear) > info.maxAcceleration)
	{
		steering.linear = glm::normalize(steering.linear);
		steering.linear *= info.maxAcceleration;
	}
	steering.angular = 0;
	return steering;
}
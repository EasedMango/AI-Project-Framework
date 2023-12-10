#pragma once
#include <variant>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "AIBehaviors.h"
#include "Common.h"
#include "Grid.h"
#include "SteeringOutput.h"
#include "AIStateMachine.h"









struct AI
{
	BehaviorStateMachine stateMachine;
	SteeringOutput steering;
};



#pragma once
#include "AI.h"
#include "AIBehaviors.h"

struct Behavior
{
	AIBehaviors::BehaviorType type;
	float weight;
};


struct BehaviorStateMachine
{
	struct State
	{

		std::vector<Behavior> behavior;
		std::vector<std::pair< std::function<bool()>, ID>> conditions;
		void AddCondition(const std::function<bool()>& condition, ID nextState)
		{
			conditions.emplace_back(condition, nextState);
		}
		void AddBehavior(AIBehaviors::BehaviorType type, const AIinfo& info, float weight = 1.0f)
		{
						behavior.emplace_back(Behavior{ type, weight, info });
		}
		State(Behavior behavior)
		{
			this->behavior.emplace_back(behavior);
		}
	};
private:
	std::unordered_map<ID, State> states;
	ID currentState = 0;
public:

	void SetState(ID stateId)
	{
		currentState = stateId;
	}
	void AddState(Behavior behavior, ID stateId = 0) {
		if (stateId == 0)
		{
			stateId = static_cast<ID>(type);
		}
		else
		{
			stateId = stateId + static_cast<ID>(AIBehaviors::BehaviorType::Flee);
		}
		states[stateId] = behavior;
	}
	void Check() {
		auto& currentStateObj = states[currentState];
		// Check each condition in the current state
		for (auto& [condition, nextState] : currentStateObj.conditions) {
			if (condition()) {
				TransitionTo(nextState);
				break; // Exit the loop once a condition is met
			}
		}
	}
	void TransitionTo(ID nextState) {
		currentState = nextState;
	}

	std::vector<Behavior> GetCurrentState()
	{
		return states[currentState].behavior;
	}
};

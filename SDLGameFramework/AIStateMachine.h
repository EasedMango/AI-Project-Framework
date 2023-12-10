#pragma once
#include "AIBehaviors.h"

struct Behavior
{
	AIBehaviors::BehaviorType type;
	AIBehaviors::BehaviorInfo info;
	float weight;
};
struct VariableContainer {
	std::unordered_map<std::string, float> floatVars;
	std::unordered_map<std::string, int> intVars;
	std::unordered_map<std::string, bool> boolVars;

	// Methods to get/set variables...
	float GetFloat(const std::string& name)
	{
		return floatVars[name];
	}
	int GetInt(const std::string& name)
	{
		return intVars[name];
	}
	bool GetBool(const std::string& name)
	{
		return boolVars[name];
	}

	void SetFloat(const std::string& name, const float value)
	{
		floatVars[name] = value;
	}
	void SetInt(const std::string& name, const int value)
	{
		intVars[name] = value;
	}
	void SetBool(const std::string& name, const bool value)
	{
		boolVars[name] = value;
	}




};

struct BehaviorStateMachine
{
	struct State
	{
		ID id;
		std::vector<Behavior> behaviors;
		std::vector<std::pair< std::function<bool(VariableContainer&)>, ID>> conditions;
		void AddCondition(const std::function<bool(VariableContainer&)>& condition, ID nextState)
		{
			conditions.emplace_back(condition, nextState);
		}
		void AddBehavior(const AIBehaviors::BehaviorType type, const AIBehaviors::BehaviorInfo& info, const float weight = 1.0f)
		{
			behaviors.emplace_back(Behavior{ type,  info ,weight });
		}
		void AddBehavior(const Behavior& behavior)
		{
			behaviors.emplace_back(behavior);
		}
		State(const Behavior& behavior, ID id) : id(id)
		{
			this->behaviors.emplace_back(behavior);
		}
		State() = default;

		//check conditions
		auto CheckConditions(VariableContainer& variables)
		{
			for (auto& [condition, nextState] : conditions) {
				if (condition(variables)) {
					return nextState;
				}
			}
			return id;
		}
	};
private:
	std::unordered_map<ID, State> states;
	ID currentState = 0;
	ID nextBehaviorID = 0;
	VariableContainer variables;
public:

	BehaviorStateMachine() = default;

	void SetState(ID stateId)
	{
		currentState = stateId;
	}
	State& AddState(Behavior behavior) {
		states[nextBehaviorID].id = nextBehaviorID;
		states[nextBehaviorID].AddBehavior(behavior);
		return states[nextBehaviorID++];
	}

	void Check() {
		if (const auto checkState = states[currentState].CheckConditions(variables); checkState != currentState)
		{
			TransitionTo(checkState);
		}
	}
	void TransitionTo(ID nextState) {
		currentState = nextState;
	}

	State& GetCurrentState()
	{
		return states[currentState];
	}
	std::vector<Behavior>& GetCurrentBehaviors()
	{
		return states[currentState].behaviors;
	}
	float GetFloat(const std::string& name)
	{
		return variables.floatVars[name];
	}
	int GetInt(const std::string& name)
	{
		return variables.intVars[name];
	}
	bool GetBool(const std::string& name)
	{
		return variables.boolVars[name];
	}

	void SetFloat(const std::string& name, const float value)
	{
		variables.floatVars[name] = value;
	}
	void SetInt(const std::string& name, const int value)
	{
		variables.intVars[name] = value;
	}
	void SetBool(const std::string& name, const bool value)
	{
		variables.boolVars[name] = value;
	}
};

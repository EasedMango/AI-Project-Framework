#pragma once
#include <memory>

#include "ISystem.h"
#include "Registry.h"
#include "../Common.h"
class SystemManager
{
private:
	std::vector<ScopePtr<ISystem>> systems;

public:

	template <typename T>
	void AddSystem() {
		systems.push_back(std::make_unique<T>());
	}
	void OnCreate(Registry& registry) const
	{
		for (auto& system : systems) {
			system->OnCreate(registry);
		}
	}
	void Update(const float& deltaTime,Registry& registry) const
	{
		for (auto& system : systems) {
			system->Update(deltaTime,registry);
		}
	}

	void OnDestroy(Registry& registry) const
	{
		for (auto& system : systems) {
			system->OnDestroy(registry);
		}
	}
};

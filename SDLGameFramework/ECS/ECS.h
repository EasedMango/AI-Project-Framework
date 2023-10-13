#pragma once
#include <memory>
#include <unordered_map>

#include "Registry.h"
#include "SparseSet.h"
#include "SystemManager.h"
#include "../Common.h"


class ECS {
private:

	ECS()
	{
		registry = Registry();
		systemManager = SystemManager();
	}

	Registry registry;
	SystemManager systemManager;

	protected:
		friend class Core;
		static std::unique_ptr<ECS> Create() {
			return std::unique_ptr<ECS>(new ECS());
		}

public:
	//delete copy and move constructors and assign operators
	ECS(const ECS&) = delete;
	ECS(ECS&&) = delete;
	ECS& operator=(const ECS&) = delete;
	ECS& operator=(ECS&&) = delete;



	Registry& GetRegistry() {
		return registry;
	}

	SystemManager& GetSystemManager() {
		return systemManager;
	}


	template <typename T>
	void AddSystem() {
		systemManager.AddSystem<T>();
	}

	void OnCreate()
	{
		systemManager.OnCreate(registry);
	}

	void Update(const float& deltaTime)
	{
		systemManager.Update( deltaTime,registry);
	}

	void OnDestroy()
	{
		systemManager.OnDestroy(registry);
	}


};

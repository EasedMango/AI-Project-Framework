#pragma once
#include <mutex>

#include "CommonECS.h"
#include "SparseSet.h"
#include "../Debug.h"

// Basic component manager which stores components in static sparse sets for each given type.
class ComponentManager {
private:

	template<typename T>
	std::mutex& GetComponentMutex()
	{
		static std::mutex mutex;
		return mutex;
	}
public:
	template <typename T>
	SparseSet<T>& GetComponentArray() {
		std::lock_guard<std::mutex> lock(GetComponentMutex<T>());
		static SparseSet<T> array;
		return array;
	}

	template<typename TComponent, typename... Args>
	TComponent& AddComponent(ID entity, Args&&... componentArgs) {
		auto& pool = GetComponentArray<TComponent>();
		TComponent component(std::forward<Args>(componentArgs)...);
		pool.Add(entity, component);
		return pool[entity];  // Access component via SparseSet's operator[]
	}

	template <typename TComponent>
	TComponent& GetComponent(ID entity) {
		auto& pool = GetComponentArray<TComponent>();
		return pool[entity];  // Access component via SparseSet's operator[]
	}
	template <typename TComponent>
	bool TryGetComponent(ID entity,TComponent& component) {
		auto& pool = GetComponentArray<TComponent>();
		if (pool.getSize() <= 0)
		{
			return false;
		}
		component = pool[entity];
		return true;// Access component via SparseSet's operator[]
	}


	template <typename TComponent>
	std::vector<TComponent>& GetAllComponents() {
		auto& pool = GetComponentArray<TComponent>();
		// Assuming a method to get all dense array elements is added to SparseSet
		return pool.GetAllDense();
	}

	template<typename TComponent>
	void RemoveComponent(ID entity) {
		auto& pool = GetComponentArray<TComponent>();
		pool.Remove(entity);
	}

	template <typename TComponent>
	bool HasComponent(ID entity) {
		auto& pool = GetComponentArray<TComponent>();
		return pool.Contains(entity);
	}

	template <typename TComponent>
	size_t GetSize() const
	{
		auto& pool = GetComponentArray<TComponent>();
		return pool.getSize();
	}
};
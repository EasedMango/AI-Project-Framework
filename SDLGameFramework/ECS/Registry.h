#pragma once
#include <algorithm>
#include <bitset>
#include <unordered_map>
#include "Query.h"
#include "ComponentManager.h"
#include "EntityManager.h"



class Registry
{
private:
	ComponentManager componentRegistry;
	EntityManager entityManager;

	friend  class Query;
	std::unordered_map<std::bitset<MAX_COMPONENTS>, std::vector<ID>> archetypeEntities;
	std::unordered_map<ID, std::bitset<MAX_COMPONENTS>> entityArchetype;



	void AddEntityToArchetype(ID entity, const std::bitset<MAX_COMPONENTS>& archetype) {


		archetypeEntities[archetype].push_back(entity);
		entityArchetype[entity] = archetype;
	}

	void RemoveEntityFromArchetype(ID entity, const std::bitset<MAX_COMPONENTS>& archetype)
	{
		auto& entities = archetypeEntities[archetype];
		const auto&& it = std::ranges::find(entities, entity);
		if (it != entities.end()) {
			std::iter_swap(it, entities.end() - 1);
			entities.pop_back();
		}

		// Remove the archetype if it has no entities
		if (entities.empty()) {
			archetypeEntities.erase(archetype);
		}

		entityArchetype.erase(entity);
	}



	void MoveEntityToArchetype(ID entity, const std::bitset<MAX_COMPONENTS>& newArchetype) {
		std::bitset<MAX_COMPONENTS> oldArchetype;

		if (auto&& oldArchetypeIt = entityArchetype.find(entity); oldArchetypeIt != entityArchetype.end()) {
			oldArchetype = oldArchetypeIt->second;
			if (oldArchetype == newArchetype) return;

			auto& oldArchetypeEntities = archetypeEntities[oldArchetype];
			const auto it = std::ranges::find(oldArchetypeEntities, entity);
			if (it != oldArchetypeEntities.end()) {
				std::iter_swap(it, oldArchetypeEntities.end() - 1);
				oldArchetypeEntities.pop_back();
			}

			if (oldArchetypeEntities.empty()) {
				archetypeEntities.erase(oldArchetype);
			}
		}

		archetypeEntities[newArchetype].push_back(entity);
		entityArchetype[entity] = newArchetype;
	}



public:
	//singleton


	//*************************QUERIES*************************//

	Query CreateQuery() {
		return (archetypeEntities);
	}

	//*************************SYSTEMS*************************//


	//*************************ENTITIES*************************//
	ID CreateEntity() {
		return entityManager.CreateEntity();
	}
	void DestroyEntity(ID entity) {
		entityManager.DestroyEntity(entity);
	}

	template<typename... ComponentTypes, typename... Args>
	ID CreateEntity(Args&&... args) {
		ID entity = entityManager.CreateEntity();
		(componentRegistry.AddComponent<ComponentTypes>(entity, std::forward<Args>(args)...), ...);
		AddEntityToArchetype(entity, GetArchetypeID<ComponentTypes...>());
		return entity;
	}


	template <typename TComponent>
	bool HasComponent(ID entity) {
		return componentRegistry.HasComponent<TComponent>(entity);
	}




	//*************************COMPONENTS*************************//

	template<typename TComponent>
	TComponent& GetComponent(ID entity) {
		return componentRegistry.GetComponent<TComponent>(entity);
	}

	template<typename TComponent>
	void RemoveComponent(ID entity) {
		if (entityArchetype.find(entity) == entityArchetype.end()) {
			// Entity does not exist
			return;
		}

		std::bitset<MAX_COMPONENTS> currentArchetype = entityArchetype[entity];

		// Unset the bit for the component being removed
		const ID componentID = ComponentTypeRegistry::GetInstance().GetTypeId<TComponent>();
		currentArchetype.set(componentID, false);

		// Move the entity to the new archetype
		MoveEntityToArchetype(entity, currentArchetype);

		// Remove the component
		componentRegistry.RemoveComponent<TComponent>(entity);
	}

	template <typename TComponent>
	std::vector<TComponent>& GetAllComponents() {
		return componentRegistry.GetAllComponents<TComponent>();
	}

	template<typename TComponent, typename... Args>
	void AddComponent(ID entity, Args&&... args) {

		// Get the current archetype for the entity
		std::bitset<MAX_COMPONENTS> currentArchetype = entityArchetype[entity];
		//if (typeid(TComponent) == typeid(OneComp)) {
		//	std::cout << "Adding entity with ID: " << entity << std::endl;
		//}
		//if (typeid(TComponent) == typeid(TwoComp)) {
		//	std::cout << "Adding entity with ID: " << entity << std::endl;
		//}
		//if (typeid(TComponent) == typeid(BoolComp)) {
		//	std::cout << "Adding entity with ID: " << entity << std::endl;
		//}
		// Set the bit for the new component
		const ID componentID = ComponentTypeRegistry::GetInstance().GetTypeId<TComponent>();
			
		currentArchetype.set(componentID, true);

		// Move the entity to the new archetype
		MoveEntityToArchetype(entity, currentArchetype);

		componentRegistry.AddComponent<TComponent>(entity, std::forward<Args>(args)...);
	}


	template <typename TComponent>
	size_t GetSize() const {
		return componentRegistry.GetSize<TComponent>();
	}

};

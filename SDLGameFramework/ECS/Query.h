#pragma once


#include <algorithm>
#include <bitset>
#include <functional>
#include <ranges>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "CommonECS.h"




class Query {
private:
	using FilterFunc = std::function<bool(const std::bitset<MAX_COMPONENTS>&)>;

	const std::unordered_map<std::bitset<MAX_COMPONENTS>, std::vector<ID>>& archetypeEntities;
	std::vector<FilterFunc> filters;  // Accumulated conditions.
	std::vector<ID> result;



public:
	Query(const std::unordered_map<std::bitset<MAX_COMPONENTS>, std::vector<ID>>& archetypes)
		: archetypeEntities(archetypes)
	{
		//matchingArchetypes.reserve(archetypes.size());  // Reserve space to avoid reallocations.
	}


	// Exact: Entities must have exactly these components
	template <typename... ComponentTypes>
	Query& Exact() {
		std::bitset<MAX_COMPONENTS> conditions = GetArchetypeID<ComponentTypes...>();
		//std::cout << "Exact conditions: " << conditions << std::endl;  // print out the bitset
		filters.emplace_back([conditions](const std::bitset<MAX_COMPONENTS>& archetype) {
			return archetype == conditions;
			});
		return *this;
	}

	// Any: Entities must have at least one of these components
	template <typename... ComponentTypes>
	Query& Any() {
		std::bitset<MAX_COMPONENTS> conditions = GetArchetypeID<ComponentTypes...>();
		//std::cout << "Any conditions: " << conditions << std::endl;  // print out the bitset
		filters.emplace_back([conditions](const std::bitset<MAX_COMPONENTS>& archetype) {
			return (archetype & conditions).any();
			});
		return *this;
	}


	// Include (or All): Entities must have all of these components (and possibly more)
	template <typename... ComponentTypes>
	Query& Include() {
		std::bitset<MAX_COMPONENTS> conditions = GetArchetypeID<ComponentTypes...>();
		//std::cout << "Include conditions: " << conditions << std::endl;  // print out the bitset
		filters.emplace_back([conditions](const std::bitset<MAX_COMPONENTS>& archetype) {
			return (archetype & conditions) == conditions;
			});
		return *this;
	}

	std::vector<ID>& Find() {
		result.clear();
		for (const auto& [archetype, entities] : archetypeEntities)
		{
		//	std::cout << "Examining archetype: " << archetype << std::endl;
			if (std::ranges::all_of(filters, [&archetype](const FilterFunc& filter) {
				return filter(archetype);
				}))
			{
			//	std::cout << "Match found. Adding entities: " << entities.size() << std::endl;
				result.reserve(result.size() + entities.size());
				result.insert(result.end(), entities.begin(), entities.end());
			}
			else
			{
				//std::cout << "No match found for this archetype." << std::endl;
			}
		}
		return result;
	}

};
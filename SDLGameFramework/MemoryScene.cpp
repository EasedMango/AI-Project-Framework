#include "MemoryScene.h"
#include "ECS/ECS.h""
#include "Camera.h"
#include "Movement.h"
#include "MovementSystem.h"
#include "Player.h"
#include "PlayerMovementSystem.h"
#include "Timing.h"
#include "Transform.h"

bool MemoryScene::OnCreate(ECS& ecs)
{

	memGui = MemoryGUI();
	auto& registry = ecs.GetRegistry();

	auto headStick = registry.CreateEntity();
	registry.AddComponent<Transform>(headStick, glm::vec3(-7.5, 0, 1), 0.f);
	registry.AddComponent<Movement>(headStick, glm::vec2(0, 0), 20.f);
	registry.AddComponent<Sprite>(headStick, renderer.CreateSprite("Square100.png"));
	registry.AddComponent<Player>(headStick);
	headStick = registry.CreateEntity();
	registry.AddComponent<Transform>(headStick, glm::vec3(-5.5, 0, 1), 0.f);
	registry.AddComponent<Movement>(headStick, glm::vec2(0, 0), 20.f);
	registry.AddComponent<Sprite>(headStick, renderer.CreateSprite("Square100.png",1,{255,0,255,255}));
	registry.AddComponent<Player>(headStick);

	headStick = registry.CreateEntity();
	registry.AddComponent<Transform>(headStick, glm::vec3(5, 0, 1), 0.f);
	registry.AddComponent<Movement>(headStick, glm::vec2(0, 0), 20.f);
	registry.AddComponent<Sprite>(headStick, renderer.CreateSprite("Square8.png"));
	registry.AddComponent<Player>(headStick);





	ecs.GetSystemManager().AddSystem<PlayerMovementSystem>();
	ecs.GetSystemManager().AddSystem<MovementSystem>();
	ecs.OnCreate();
	return true;
}

void MemoryScene::OnDestroy(ECS& ecs)
{
	ecs.OnDestroy();
}

void MemoryScene::Update(const float& deltaTime, ECS& ecs)
{
	ecs.Update(deltaTime);
	memGui();
}

void MemoryScene::Render(Registry& registry) const
{


	renderer.BeginFrame();
	{
		// Create a temporary container to hold the entities
		std::vector<std::tuple<float, ID>> zOrderedEntities;

		// Populate the container
		for (auto que = registry.CreateQuery().Include<Transform, Sprite>(); const auto & entity : que.Find()) {
			auto& transform = registry.GetComponent<Transform>(entity);
			zOrderedEntities.emplace_back(transform.position.z, entity);
		}

		// Sort the container based on the z-value
		std::ranges::sort(zOrderedEntities,
			[](const auto& a, const auto& b) {
				return std::get<0>(a) < std::get<0>(b);  // Sort in ascending order of z-value
			});

		// Now render the entities in the sorted order
		for (const auto& item : zOrderedEntities) {
			const auto& entity = std::get<1>(item);
			auto& transform = registry.GetComponent<Transform>(entity);
			auto& sprite = registry.GetComponent<Sprite>(entity);
			renderer.RenderSprite(sprite, transform.position, transform.rotation);
		}
		renderer.GetGUI().Demo();
		
	}
	renderer.EndFrame();
}


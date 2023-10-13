#include "TestScene.h"

#include "AI.h"
#include "AISystem.h"
#include "Body.h"
#include "ECS/ECS.h"
#include "Camera.h"
#include "Player.h"
#include "PlayerMovementSystem.h"
#include "Timing.h"
#include "Transform.h"
#include "BodySystem.h"

bool TestScene::OnCreate(ECS& ecs)
{


	auto& registry = ecs.GetRegistry();
	/*auto headStickAI = registry.CreateEntity();
	registry.AddComponent<Transform>(headStickAI);
	registry.AddComponent<Body>(headStickAI);
	registry.AddComponent<Sprite>(headStickAI, renderer.CreateSprite("Head_With_A_Stick.png"));
	registry.AddComponent<AI>(headStickAI, AIBehaviors::BehaviorType::Arrive,-1, SeekInfo(10), ArriveInfo(10, 10, 5, 2, 4));*/
	auto headStick = registry.CreateEntity();
	registry.AddComponent<Transform>(headStick);
	registry.AddComponent<Body>(headStick);
	registry.AddComponent<Sprite>(headStick, renderer.CreateSprite("Head_With_A_Stick.png"));
	registry.AddComponent<Player>(headStick);

	auto headStick2 = registry.CreateEntity();
	registry.AddComponent<Transform>(headStick2);
	registry.AddComponent<Sprite>(headStick2, renderer.CreateSprite("Head_With_A_Stick.png"));

	auto headStickAI = registry.CreateEntity();
	registry.AddComponent<Transform>(headStickAI);
	registry.AddComponent<Body>(headStickAI);
	registry.AddComponent<Sprite>(headStickAI, renderer.CreateSprite("Head_With_A_Stick.png"));
	registry.AddComponent<AI>(headStickAI, AIBehaviors::BehaviorType::Arrive, headStick, SeekInfo(10), ArriveInfo(10, 10, 5, 2, 4));


	//Create circle of heads




	ecs.GetSystemManager().AddSystem<PlayerMovementSystem>();
	ecs.GetSystemManager().AddSystem<AISystem>();
	ecs.GetSystemManager().AddSystem<BodySystem>();
	
	ecs.OnCreate();
	return true;
}

void TestScene::OnDestroy(ECS& ecs)
{
	ecs.OnDestroy();
}

void TestScene::Update(const float& deltaTime, ECS& ecs)
{
	ecs.Update(deltaTime);
}

void TestScene::Render(Registry& registry) const
{


	renderer.BeginFrame();
	{
		// Create a temporary container to hold the entities
		std::vector<std::tuple<float, ID>> zOrderedEntities;

		// Populate the container
		for (auto que = registry.CreateQuery().Include<Transform, Sprite>(); const auto & entity : que.Find()) {
			auto& transform = registry.GetComponent<Transform>(entity);
			zOrderedEntities.emplace_back(transform.pos.z, entity);
		}

		// Sort the container based on the z-value
		std::ranges::sort(zOrderedEntities,
			[](const auto& a, const auto& b) {
				return std::get<0>(a) < std::get<0>(b);  // Sort in ascending order of z-value
			});
		ImGui::Begin("Test");
		// Now render the entities in the sorted order
		for (const auto& item : zOrderedEntities) {
			const auto& entity = std::get<1>(item);
			auto& transform = registry.GetComponent<Transform>(entity);
			ImGui::Text("ID: %i Pos: x%f, y%f, z%f",entity,transform.pos.x, transform.pos.y, transform.pos.z);
			auto& sprite = registry.GetComponent<Sprite>(entity);
			renderer.RenderSprite(sprite, transform.pos, transform.rot);
		}
		for(auto que = registry.CreateQuery().Include<Player,Transform>(); const auto & entity : que.Find())
		{
			auto& transform = registry.GetComponent<Transform>(entity);
			ImGui::Text("Player Pos: x%f, y%f, z%f", transform.pos.x, transform.pos.y, transform.pos.z);
		}



		ImGui::Text("Hello, world!");
		ImGui::End();
	}
	renderer.EndFrame();
}


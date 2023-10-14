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
#include "SteeringSystem.h"


bool TestScene::OnCreate(ECS& ecs)
{


	auto& registry = ecs.GetRegistry();


	auto headStick = registry.CreateEntity();
	registry.AddComponent<Transform>(headStick);
	registry.AddComponent<Body>(headStick);
	registry.AddComponent<Sprite>(headStick, renderer.CreateSprite("Head_With_A_Stick.png"));
	registry.AddComponent<Player>(headStick);


	const auto headStickSeekAI = registry.CreateEntity();
	registry.AddComponent<Transform>(headStickSeekAI);
	registry.AddComponent<Body>(headStickSeekAI, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1, 0, 0, 0, 2, 50, 3, 3, 1);
	registry.AddComponent<Sprite>(headStickSeekAI, renderer.CreateSprite("Head_With_A_Stick.png"));
	registry.AddComponent<AI>(headStickSeekAI, AIBehaviors::BehaviorType::Seek, headStick, SteeringOutput());
	registry.AddComponent<SeekInfo>(headStickSeekAI, 1);

	const auto headStickArriveAI = registry.CreateEntity();
	registry.AddComponent<Transform>(headStickArriveAI);
	registry.AddComponent<Body>(headStickArriveAI, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1, 0, 0, 0, 2, 50, 3, 3, 1);
	registry.AddComponent<Sprite>(headStickArriveAI, renderer.CreateSprite("Head_With_A_Stick.png"));
	registry.AddComponent<AI>(headStickArriveAI, AIBehaviors::BehaviorType::Arrive, headStick, SteeringOutput());
	registry.AddComponent<ArriveInfo>(headStickArriveAI );

	const auto CareAI = registry.CreateEntity();
	registry.AddComponent<Transform>(CareAI);
	registry.AddComponent<Body>(CareAI, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1, 0, 0, 0, 5, 50, 3, 3, 1);
	registry.AddComponent<Sprite>(CareAI, renderer.CreateSprite("Care.png"));
	registry.AddComponent<AI>(CareAI, AIBehaviors::BehaviorType::Chase, headStick, SteeringOutput());
	registry.AddComponent<ChaseInfo>(CareAI);

	//Create circle of heads



	ecs.GetSystemManager().AddSystem<AISystem>();
	ecs.GetSystemManager().AddSystem<SteeringSystem>();
	ecs.GetSystemManager().AddSystem<PlayerMovementSystem>();
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
		{
			ImGui::Begin("Test");
			// Now render the entities in the sorted order
			for (const auto& item : zOrderedEntities) {
				const auto& entity = std::get<1>(item);
				auto& transform = registry.GetComponent<Transform>(entity);
				ImGui::Text("ID: %i Pos: x%f, y%f, z%f", entity, transform.pos.x, transform.pos.y, transform.pos.z);
				auto& sprite = registry.GetComponent<Sprite>(entity);
				renderer.RenderSprite(sprite, transform.pos, transform.rot);
			}
			if (ImGui::TreeNode("Bodies")) {
				for (auto que = registry.CreateQuery().Include<Transform, Body>(); const auto & entity : que.Find())
				{

					std::string s = ("Entity: "); s.append(std::to_string(entity));
					if (ImGui::TreeNode(s.c_str()))
					{
						auto& transform = registry.GetComponent<Transform>(entity);
						ImGui::Text("Transform:\n Pos: x%f, y%f, z%f\n Rot: %f", transform.pos.x, transform.pos.y, transform.pos.z, transform.rot);

						auto& body = registry.GetComponent<Body>(entity);
						ImGui::Text("\nBody:\n"
							" Vel     : x%f, y%f, z%f\n"
							" Accel   : x%f, y%f, z%f\n"
							" RotVel  : %f\n"
							" RotAccel: %f",
							body.vel.x, body.vel.y, body.vel.z,
							body.accel.x, body.accel.y, body.accel.z,
							body.rotation,
							body.angular);
						if (ImGui::TreeNode("  Other Values"))
						{
							ImGui::Text("Max Speed: %f\n"
								"  Max Acceleration: %f\n"
								"  Max Rotation: %f\n"
								"  Max Angular: %f\n"
								"  Orientation: %f\n"
								"  Mass: %f\n"
								"  Radius: %f",
								body.maxSpeed,
								body.maxAcceleration,
								body.maxRotation,
								body.maxAngular,
								body.orientation,
								body.mass,
								body.radius);
							ImGui::TreePop();
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}


		ImGui::End();
	}
	renderer.EndFrame();
}


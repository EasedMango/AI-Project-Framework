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
#include "CameraComp.h"
#include "Collider.h"
#include "CollisionSystem.h"
#include "Core.h"
#include "Grid.h"
#include "InputHandler.h"
#include "SpatialHash.h"
#include "SteeringSystem.h"
#include "SystemsLocator.h"
#include "Timer.h"
#include "UIClasses.h"


bool TestScene::OnCreate(ECS& ecs)
{


	auto& registry = ecs.GetRegistry();

	const auto bg = registry.CreateEntity();
	registry.AddComponent<Transform>(bg, glm::vec3(0.f, 0.f, -1.f));
	registry.AddComponent<Sprite>(bg, renderer.CreateSprite("GrossBG.png", 4));


	auto headStick = registry.CreateEntity();
	registry.AddComponent<Transform>(headStick);
	registry.AddComponent<Body>(headStick);
	registry.AddComponent<Sprite>(headStick, renderer.CreateSprite("Square.png"));
	registry.AddComponent<Player>(headStick);
	registry.AddComponent<CameraComp>(headStick);
	registry.AddComponent<Collider>(headStick, ColliderShape::BoxCollider, 1);
	registry.AddComponent<BoxCollider>(headStick, 0.5f, 0.5f, 0, glm::vec2(0, 0));

	const auto headStickSeekAMove = registry.CreateEntity();
	registry.AddComponent<Transform>(headStickSeekAMove);
	registry.AddComponent<Body>(headStickSeekAMove, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), 1, 0, 0, 0, 2, 50, 3, 3, 1);
	registry.AddComponent<Sprite>(headStickSeekAMove, renderer.CreateSprite("Square.png"));
	registry.AddComponent<Collider>(headStickSeekAMove, ColliderShape::BoxCollider, 1);
	registry.AddComponent<BoxCollider>(headStickSeekAMove, 0.5f, 0.5f, 0, glm::vec2(0, 0));
	registry.AddComponent<AI>(headStickSeekAMove, AIBehaviors::BehaviorType::Wander, headStickSeekAMove, SteeringOutput());
	registry.AddComponent<WanderInfo>(headStickSeekAMove, 5, glm::vec2(0, 0), 1, std::vector<Tile*>(), 0);




	/*const auto headStickSeekAI = registry.CreateEntity();
	registry.AddComponent<Transform>(headStickSeekAI);
	registry.AddComponent<Body>(headStickSeekAI, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), 1, 0, 0, 0, 2, 50, 3, 3, 1);
	registry.AddComponent<Sprite>(headStickSeekAI, renderer.CreateSprite("Head_With_A_Stick.png"));
	registry.AddComponent<AI>(headStickSeekAI, AIBehaviors::BehaviorType::Seek, headStick, SteeringOutput());
	registry.AddComponent<SeekInfo>(headStickSeekAI, 1);
	registry.AddComponent<Collider>(headStickSeekAI, ColliderShape::BoxCollider, 1);
	registry.AddComponent<BoxCollider>(headStickSeekAI, 1, 1, 0, glm::vec2(0, 0));

	const auto headStickArriveAI = registry.CreateEntity();
	registry.AddComponent<Transform>(headStickArriveAI);
	registry.AddComponent<Body>(headStickArriveAI, glm::vec3(4, 0, 0), glm::vec3(0, 0, 0), 1, 0, 0, 0, 2, 50, 3, 3, 1);
	registry.AddComponent<Sprite>(headStickArriveAI, renderer.CreateSprite("Head_With_A_Stick.png"));
	registry.AddComponent<AI>(headStickArriveAI, AIBehaviors::BehaviorType::Flee, headStick, SteeringOutput());
	registry.AddComponent<FleeInfo>(headStickArriveAI);
	registry.AddComponent<Collider>(headStickArriveAI, ColliderShape::BoxCollider, 1);
	registry.AddComponent<BoxCollider>(headStickArriveAI, 1, 1, 0, glm::vec2(0, 0));

	const auto CareAI = registry.CreateEntity();
	registry.AddComponent<Transform>(CareAI);
	registry.AddComponent<Body>(CareAI, glm::vec3(6, 0, 0), glm::vec3(0, 0, 0), 1, 0, 0, 0, 5, 50, 3, 3, 1);
	registry.AddComponent<Sprite>(CareAI, renderer.CreateSprite("Care.png"));
	registry.AddComponent<AI>(CareAI, AIBehaviors::BehaviorType::Chase, headStick, SteeringOutput());
	registry.AddComponent<ChaseInfo>(CareAI);
	registry.AddComponent<Collider>(CareAI, ColliderShape::BoxCollider, 1);
	registry.AddComponent<BoxCollider>(CareAI, 1, 1, 0, glm::vec2(0, 0));*/



	////Create circle of heads
	//const auto headStickPatrolAI = registry.CreateEntity();
	//registry.AddComponent<Transform>(headStickPatrolAI, glm::vec3(-6, 4, 0), 0);
	//registry.AddComponent<Body>(headStickPatrolAI, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1, 0, 0, 0, 2, 50, 3, 3, 1);
	//registry.AddComponent<Sprite>(headStickPatrolAI, renderer.CreateSprite("Head_With_A_Stick.png"));
	//registry.AddComponent<AI>(headStickPatrolAI, AIBehaviors::BehaviorType::Patrol, headStick, SteeringOutput());
	//registry.AddComponent<PatrolInfo>(headStickPatrolAI, glm::vec3(-6, 4, 0), glm::vec3(6, 4, 0), 0);
	//registry.AddComponent<Collider>(headStickPatrolAI, ColliderShape::BoxCollider, 1);
	//registry.AddComponent<BoxCollider>(headStickPatrolAI, 1, 1, 0, glm::vec2(0, 0));

		//create start button
	//const auto startButton = registry.CreateEntity<UIElement>();
	//registry.AddComponent<Button>(startButton);
	//registry.AddComponent<Transform>(startButton, glm::vec3{ 0,0,-100 }, 0);
	//registry.AddComponent<Sprite>(startButton, renderer.CreateSprite("Square.png"));

	//registry.AddComponent<UIElement>(startButton, true);

	//registry.GetComponent<Button>(startButton).AddCallback([]()
	//	{
	//		Core::Pause();
	//	});


	//auto&& buttonCheck = [&](const SDL_Event& sdlEvent)
	//	{

	//		int x = sdlEvent.button.x;
	//		int y = sdlEvent.button.y;


	//		auto que = registry.CreateQuery().Include<UIElement, Button, Transform, Sprite>();

	//		for (auto&& entity : que.Find())
	//		{
	//			auto&& button = registry.GetComponent<Button>(entity);
	//			auto&& transform = registry.GetComponent<Transform>(entity);
	//			auto&& sprite = registry.GetComponent<Sprite>(entity);
	//			auto&& uiElement = registry.GetComponent<UIElement>(entity);
	//			if (!uiElement.active)
	//			{
	//				return;
	//			}
	//			if (x > transform.pos.x - sprite.width / 2 && x < transform.pos.x + sprite.width / 2 &&
	//				y > transform.pos.y - sprite.height / 2 && y < transform.pos.y + sprite.height / 2)
	//			{

	//				uiElement.active = false;
	//				button.OnClick();


	//			}
	//		}
	//	};

	//InputHandler::Instance().RegisterMouseButtonDownCallback(0, buttonCheck);
	registry.CreateEntity<SpatialHash>();


	ecs.GetSystemManager().AddSystem<AISystem>();
	ecs.GetSystemManager().AddSystem<SteeringSystem>();
	ecs.GetSystemManager().AddSystem<PlayerMovementSystem>();
	ecs.GetSystemManager().AddSystem<BodySystem>();
	ecs.GetSystemManager().AddSystem<CollisionSystem>();
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

		static auto camera = registry.CreateQuery().Include<CameraComp>().Find()[0];
		auto posCam = registry.GetComponent<Transform>(camera).pos + glm::vec3(registry.GetComponent<CameraComp>(camera).position, 0.f);
		renderer.GetCurrentCamera()->SetPosition(posCam);
		renderer.GetCurrentCamera()->SetZoom(registry.GetComponent<CameraComp>(camera).zoom);

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
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Camera Position x%f, y%f, z%f", posCam.x, posCam.y, posCam.z);
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


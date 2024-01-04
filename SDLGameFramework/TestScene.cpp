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
#include "TileMap.h"
#include "Timer.h"
#include "UIClasses.h"
#include "AI.h"
#include "Vision.h"
#include "VisionSystem.h"

ID playerID;
ID aiID;
ID aiID2;


bool TestScene::OnCreate()
{
	auto& registry = ecs->GetRegistry();

	const auto bg = registry.CreateEntity();
	registry.AddComponent<Transform>(bg, glm::vec2(9.5f, 7.f));
	registry.AddComponent<Sprite>(bg, renderer->CreateSprite("DD_Test_Map2.png", 0));


	playerID = registry.CreateEntity();
	registry.AddComponent<Transform>(playerID, glm::vec2(4, 2));
	registry.AddComponent<Body>(playerID);
	registry.AddComponent<Sprite>(playerID, renderer->CreateSprite("Main_char64.png", 1));
	registry.AddComponent<Player>(playerID);
	registry.AddComponent<CameraComp>(playerID);
	registry.AddComponent<Collider>(playerID, ColliderShape::CircleCollider, 1);
	registry.AddComponent<CircleCollider>(playerID, glm::vec2(0, 0), 0.5f);

	{
		aiID = registry.CreateEntity();
		registry.AddComponent<Transform>(aiID, glm::vec2(3, 4));
		registry.AddComponent<Body>(aiID, glm::vec2(0, 0), glm::vec2(0, 0), 1, 0, 0, 0, 2, 50, 3, 3, 1);
		registry.AddComponent<Sprite>(aiID, renderer->CreateSprite("Care.png", 1));
		registry.AddComponent<Collider>(aiID, ColliderShape::CircleCollider, 1);
		registry.AddComponent<CircleCollider>(aiID, glm::vec2(0, 0), 0.5f);
		registry.AddComponent<Vision>(aiID, playerID, 5.f, 0.5f);
		auto& stateMachine = registry.AddComponent<AI>(aiID).stateMachine;
		auto& seekState = stateMachine.AddState({ AIBehaviors::BehaviorType::Seek, AIBehaviors::SeekInfo{playerID,3.f},3.f });
		seekState.AddBehavior(AIBehaviors::BehaviorType::AvoidCollision, AIBehaviors::AvoidCollisionInfo{ 1.f }, 0.25f);
		auto& wanderState = stateMachine.AddState({ AIBehaviors::BehaviorType::Wander, AIBehaviors::WanderInfo{10,glm::vec2(9.5f,7.f),1,std::vector<Tile>(),0},1.f });
		wanderState.AddCondition([](VariableContainer& variables)
			{
				const bool change = (variables.GetBool("Visible")) && (variables.GetFloat("Distance") < 5.f);

				if (change)
				{
					printf("Player Visible\n");
				}


				return change;
			}, seekState.id);
		seekState.AddCondition([](VariableContainer& variables)
			{
				const bool change = (!variables.GetBool("Visible"));

				if (change)
				{
					printf("Player Not Visible\n");
				}

				return change;
			}, wanderState.id);
	}

	{
		aiID2 = registry.CreateEntity();
		registry.AddComponent<Transform>(aiID2, glm::vec2(5, 4));
		registry.AddComponent<Body>(aiID2, glm::vec2(0, 0), glm::vec2(0, 0), 1, 0, 0, 0, 2, 50, 3, 3, 1);
		registry.AddComponent<Sprite>(aiID2, renderer->CreateSprite("Bride.png", 1));
		registry.AddComponent<Collider>(aiID2, ColliderShape::CircleCollider, 1);
		registry.AddComponent<CircleCollider>(aiID2, glm::vec2(0, 0), 0.5f);
		registry.AddComponent<Vision>(aiID2, playerID, 5.f, 0.5f);
		auto& stateMachine = registry.AddComponent<AI>(aiID2).stateMachine;
		auto& fleeState = stateMachine.AddState({ AIBehaviors::BehaviorType::Flee, AIBehaviors::FleeInfo{playerID,3.f},3.f });
		fleeState.AddBehavior(AIBehaviors::BehaviorType::AvoidCollision, AIBehaviors::AvoidCollisionInfo{ 1.f }, 0.25f);
		auto& wanderState = stateMachine.AddState({ AIBehaviors::BehaviorType::Wander, AIBehaviors::WanderInfo{10,glm::vec2(9.5f,7.f),1,std::vector<Tile>(),0},1.f });
		wanderState.AddCondition([](VariableContainer& variables)
			{
				const bool change = (variables.GetBool("Visible")) && (variables.GetFloat("Distance") < 5.f);

				if (change)
				{
					printf("Player Is Scary\n");
				}


				return change;
			}, fleeState.id);
		fleeState.AddCondition([](VariableContainer& variables)
			{
				const bool change = (!variables.GetBool("Visible"));

				if (change)
				{
					printf("Player Is Not Scary\n");
				}

				return change;
			}, wanderState.id);
	}

	//aiID = registry.CreateEntity();
	//registry.AddComponent<Transform>(aiID, glm::vec2(4, 4));
	//registry.AddComponent<Body>(aiID, glm::vec2(0, 0), glm::vec2(0, 0), 1, 0, 0, 0, 2, 50, 3, 3, 1);
	//registry.AddComponent<Sprite>(aiID, renderer->CreateSprite("Care.png", 1));
	//registry.AddComponent<Collider>(aiID, ColliderShape::CircleCollider, 1);
	//registry.AddComponent<CircleCollider>(aiID, glm::vec2(0, 0), 0.5f);
	//registry.AddComponent<Vision>(aiID, playerID, 5.f, 0.5f);
	//auto& stateMachine = registry.AddComponent<AI>(aiID).stateMachine;

	//// second character to use flee state for gail
	//aiID2 = registry.CreateEntity();
	//registry.AddComponent<Transform>(aiID2, glm::vec2(4, 4));
	//registry.AddComponent<Body>(aiID2, glm::vec2(0, 0), glm::vec2(0, 0), 1, 0, 0, 0, 2, 50, 3, 3, 1);
	//registry.AddComponent<Sprite>(aiID2, renderer->CreateSprite("Bride.png", 1));
	//registry.AddComponent<Collider>(aiID2, ColliderShape::CircleCollider, 1);
	//registry.AddComponent<CircleCollider>(aiID2, glm::vec2(0, 0), 0.5f);
	//registry.AddComponent<Vision>(aiID2, playerID, 5.f, 0.5f);
	////auto& stateMachine = registry.AddComponent<AI>(aiID2).stateMachine;

	//auto& seekState = stateMachine.AddState({ AIBehaviors::BehaviorType::Seek, AIBehaviors::SeekInfo{playerID,3.f},3.f });
	//seekState.AddBehavior(AIBehaviors::BehaviorType::AvoidCollision, AIBehaviors::AvoidCollisionInfo{ 1.f }, 0.25f);
	//auto& wanderState = stateMachine.AddState({ AIBehaviors::BehaviorType::Wander, AIBehaviors::WanderInfo{10,glm::vec2(9.5f,7.f),1,std::vector<Tile>(),0},1.f });

	//// added flee state
	//auto& fleeState = stateMachine.AddState({ AIBehaviors::BehaviorType::Flee, AIBehaviors::FleeInfo{playerID, 2.0f} });
	//fleeState.AddBehavior(AIBehaviors::BehaviorType::AvoidCollision, AIBehaviors::AvoidCollisionInfo{ 1.f }, 0.25f);

	//wanderState.AddCondition([](VariableContainer& variables)
	//	{
	//		const bool change = (variables.GetBool("Visible")) && (variables.GetFloat("Distance") < 5.f);

	//		if (change)
	//		{
	//			printf("Player Visible\n");
	//		}


	//		return change;
	//	}, seekState.id);
	//seekState.AddCondition([](VariableContainer& variables)
	//	{
	//		const bool change = (!variables.GetBool("Visible"));

	//		if (change)
	//		{
	//			printf("Player Not Visible\n");
	//		}

	//		return change;
	//	}, wanderState.id);

	//// flee condition for gail
	//wanderState.AddCondition([](VariableContainer& variables)
	//	{
	//		const bool change = (variables.GetBool("Visible")) && (variables.GetFloat("Distance") < 5.f);

	//		if (change)
	//		{
	//			printf("Player Isn't Scary\n");
	//		}


	//		return change;
	//	}, fleeState.id);
	//fleeState.AddCondition([](VariableContainer& variables)
	//	{
	//		const bool change = (variables.GetBool("Visible")) && (variables.GetBool("Distance") < 3.0f);

	//		if (change) {
	//			printf("Player Is Scary\n");
	//		}

	//		return change;
	//	}, wanderState.id);




	const auto& grid = TileMap::Instance("Map/64map.tsx").GetGrid();
	const auto tiles = grid.GetTiles();
	for (int i = 0; i < grid.GetWidth(); ++i)
	{
		for (int j = 0; j < grid.GetHeight(); ++j)
		{
			if (auto&& tile = tiles[i][j]; tile->wall)
			{
				//auto floor = registry.CreateEntity();
				//registry.AddComponent<Sprite>(floor, renderer->CreateSprite("Square.png",1,glm::vec4(255,255,255,255)));
				//registry.AddComponent<Transform>(floor, glm::vec2(tile->x, tile->y, -1.f));
				const auto wall = registry.CreateEntity();
				//registry.AddComponent<Sprite>(wall, renderer->CreateSprite("Square.png", 1, glm::vec4(0, 0, 0, 255)));
				registry.AddComponent<Transform>(wall, glm::vec2(tile->x, tile->y));
				registry.AddComponent<Collider>(wall, ColliderShape::BoxCollider, 1, false);
				registry.AddComponent<BoxCollider>(wall, 0.5f, 0.5f, 0, glm::vec2(0, 0));
				std::cout << "Wall: " << wall << std::endl;
			}

		}
	}
	registry.CreateEntity<SpatialHash>();


	ecs->GetSystemManager().AddSystem<AISystem>();
	ecs->GetSystemManager().AddSystem<VisionSystem>();
	ecs->GetSystemManager().AddSystem<SteeringSystem>();
	ecs->GetSystemManager().AddSystem<PlayerMovementSystem>();
	ecs->GetSystemManager().AddSystem<BodySystem>();
	ecs->GetSystemManager().AddSystem<CollisionSystem>();
	ecs->OnCreate();
	return true;
}

void TestScene::OnDestroy()
{
	ecs->OnDestroy();
}

void TestScene::Update(const float& deltaTime)
{
	ecs->Update(deltaTime);

	const float distance = glm::distance(ecs->GetRegistry().GetComponent<Transform>(playerID).pos, ecs->GetRegistry().GetComponent<Transform>(aiID).pos);

	ecs->GetRegistry().GetComponent<AI>(aiID).stateMachine.SetFloat("Distance", distance);
}

void TestScene::Render() const
{
	auto& registry = ecs->GetRegistry();

	renderer->BeginFrame();
	{
		// Create a temporary container to hold the entities
		std::vector<std::tuple<float, ID>> zOrderedEntities;

		static auto camera = registry.CreateQuery().Include<CameraComp>().Find()[0];
		const auto posCam = registry.GetComponent<Transform>(camera).pos + (registry.GetComponent<CameraComp>(camera).position);
		renderer->GetCurrentCamera()->SetPosition(posCam);
		renderer->GetCurrentCamera()->SetZoom(registry.GetComponent<CameraComp>(camera).zoom);

		// Populate the container
		for (auto que = registry.CreateQuery().Include<Sprite>(); const auto & entity : que.Find()) {

			auto& transform = registry.GetComponent<Sprite>(entity);
			zOrderedEntities.emplace_back(transform.priority, entity);
		}

		// Sort the container based on the z-value
		std::ranges::sort(zOrderedEntities,
			[](const auto& a, const auto& b) {
				return std::get<0>(a) < std::get<0>(b);  // Sort in ascending order of z-value
			});
		{
			ImGui::Begin("Test");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Camera Position x%f, y%f", posCam.x, posCam.y);
			// Now render the entities in the sorted order
			for (const auto& item : zOrderedEntities) {
				const auto& entity = std::get<1>(item);
				auto& transform = registry.GetComponent<Transform>(entity);
				ImGui::Text("ID: %i Pos: x%f, y%f", entity, transform.pos.x, transform.pos.y);
				const auto& sprite = registry.GetComponent<Sprite>(entity);
				renderer->RenderSprite(sprite, transform.pos, transform.rot);
			}
			if (ImGui::TreeNode("Bodies")) {
				for (auto que = registry.CreateQuery().Include<Transform, Body>(); const auto & entity : que.Find())
				{

					std::string s = ("Entity: "); s.append(std::to_string(entity));
					if (ImGui::TreeNode(s.c_str()))
					{
						const auto& transform = registry.GetComponent<Transform>(entity);
						ImGui::Text("Transform:\n Pos: x%f, y%f\n Rot: %f", transform.pos.x, transform.pos.y, transform.rot);

						const auto& body = registry.GetComponent<Body>(entity);
						ImGui::Text("\nBody:\n"
							" Vel     : x%f, y%f\n"
							" Accel   : x%f, y%f\n"
							" RotVel  : %f\n"
							" RotAccel: %f",
							body.vel.x, body.vel.y,
							body.accel.x, body.accel.y,
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
	renderer->EndFrame();
}


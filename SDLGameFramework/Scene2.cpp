//#include "Scene2.h"
//
//#include "AI.h"
//#include "AISystem.h"
//#include "Body.h"
//#include "ECS/ECS.h"
//#include "Camera.h"
//#include "Player.h"
//#include "PlayerMovementSystem.h"
//#include "Timing.h"
//#include "Transform.h"
//#include "BodySystem.h"
//#include "CameraComp.h"
//#include "Collider.h"
//#include "CollisionSystem.h"
//#include "Core.h"
//#include "Grid.h"
//#include "InputHandler.h"
//#include "SpatialHash.h"
//#include "SteeringSystem.h"
//#include "SystemsLocator.h"
//#include "TileMap.h"
//#include "Timer.h"
//#include "UIClasses.h"
//
//
//
//
//bool Scene2::OnCreate()
//{
//
//
//	auto& registry = ecs->GetRegistry();
//
//	const auto bg = registry.CreateEntity();
//	registry.AddComponent<Transform>(bg, glm::vec2(9.5f, 7.f, -1.f));
//	registry.AddComponent<Sprite>(bg, renderer->CreateSprite("DD_Test_Map2.png", TODO));
//
//
//	const auto headStick = registry.CreateEntity();
//	registry.AddComponent<Transform>(headStick, glm::vec2(4, 2, 0));
//	auto& transform = registry.GetComponent<Transform>(headStick);
//	registry.AddComponent<Body>(headStick);
//	registry.AddComponent<Sprite>(headStick, renderer->CreateSprite("Main_char64.png", TODO));
//	registry.AddComponent<Player>(headStick);
//	registry.AddComponent<CameraComp>(headStick);
//	registry.AddComponent<Collider>(headStick, ColliderShape::CircleCollider, 1);
//	registry.AddComponent<CircleCollider>(headStick, glm::vec2(0, 0), 0.5f);
//
//	const auto headStickSeekAMove = registry.CreateEntity();
//	registry.AddComponent<Transform>(headStickSeekAMove, glm::vec2(4, 4, 0));
//	auto& transform2 = registry.GetComponent<Transform>(headStickSeekAMove);
//	registry.AddComponent<Body>(headStickSeekAMove, glm::vec2(0, 0, 0), glm::vec2(0, 0, 0), 1, 0, 0, 0, 2, 50, 3, 3, 1);
//	registry.AddComponent<Sprite>(headStickSeekAMove, renderer->CreateSprite("Care.png", TODO));
//	registry.AddComponent<Collider>(headStickSeekAMove, ColliderShape::CircleCollider, 1);
//	registry.AddComponent<CircleCollider>(headStickSeekAMove, glm::vec2(0, 0), 0.5f);
//	//registry.AddComponent<AI>(headStickSeekAMove, std::vector<WeightedBehavior>{ {AIBehaviors::BehaviorType::Wander, 1} });
//	//registry.AddComponent<WanderInfo>(headStickSeekAMove, 10, glm::vec2(9.5f, 7.f), 1, std::vector<Tile*>(), 0);
//
//
//
//
//
//
//
//	const auto& grid = TileMap::Instance("Map/64map.tsx").GetGrid();
//	const auto tiles = grid.GetTiles();
//	for (int i = 0; i < grid.GetWidth(); ++i)
//	{
//		for (int j = 0; j < grid.GetHeight(); ++j)
//		{
//			if (auto&& tile = tiles[i][j]; tile->wall)
//			{
//				//auto floor = registry.CreateEntity();
//				//registry.AddComponent<Sprite>(floor, renderer->CreateSprite("Square.png",1,glm::vec4(255,255,255,255)));
//				//registry.AddComponent<Transform>(floor, glm::vec2(tile->x, tile->y, -1.f));
//				const auto wall = registry.CreateEntity();
//				//registry.AddComponent<Sprite>(wall, renderer->CreateSprite("Square.png", 1, glm::vec4(0, 0, 0, 255)));
//				registry.AddComponent<Transform>(wall, glm::vec2(tile->x, tile->y, -1.f));
//				registry.AddComponent<Collider>(wall, ColliderShape::BoxCollider, 1, false);
//				registry.AddComponent<BoxCollider>(wall, 0.5f, 0.5f, 0, glm::vec2(0, 0));
//			}
//
//		}
//	}
//
//
//
//	registry.CreateEntity<SpatialHash>();
//
//
//	ecs->GetSystemManager().AddSystem<AISystem>();
//	ecs->GetSystemManager().AddSystem<SteeringSystem>();
//	ecs->GetSystemManager().AddSystem<PlayerMovementSystem>();
//	ecs->GetSystemManager().AddSystem<BodySystem>();
//	ecs->GetSystemManager().AddSystem<CollisionSystem>();
//	ecs->OnCreate();
//	return true;
//}
//
//void Scene2::OnDestroy()
//{
//	ecs->OnDestroy();
//}
//
//void Scene2::Update(const float& deltaTime)
//{
//	ecs->Update(deltaTime);
//}
//
//void Scene2::Render() const
//{
//
//
//	auto& registry = ecs->GetRegistry();
//	renderer->BeginFrame();
//	{
//		// Create a temporary container to hold the entities
//		std::vector<std::tuple<float, ID>> zOrderedEntities;
//
//		static auto camera = registry.CreateQuery().Include<CameraComp>().Find()[0];
//		const auto posCam = registry.GetComponent<Transform>(camera).pos + glm::vec2(registry.GetComponent<CameraComp>(camera).position, 0.f);
//		renderer->GetCurrentCamera()->SetPosition(posCam);
//		renderer->GetCurrentCamera()->SetZoom(registry.GetComponent<CameraComp>(camera).zoom);
//
//		// Populate the container
//		for (auto que = registry.CreateQuery().Include<Transform, Sprite>(); const auto & entity : que.Find()) {
//			auto& transform = registry.GetComponent<Transform>(entity);
//			zOrderedEntities.emplace_back(transform.pos.z, entity);
//		}
//
//		// Sort the container based on the z-value
//		std::ranges::sort(zOrderedEntities,
//			[](const auto& a, const auto& b) {
//				return std::get<0>(a) < std::get<0>(b);  // Sort in ascending order of z-value
//			});
//		{
//			ImGui::Begin("Test");
//			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//			ImGui::Text("Camera Position x%f, y%f, z%f", posCam.x, posCam.y, posCam.z);
//			// Now render the entities in the sorted order
//			for (const auto& item : zOrderedEntities) {
//				const auto& entity = std::get<1>(item);
//				auto& transform = registry.GetComponent<Transform>(entity);
//				ImGui::Text("ID: %i Pos: x%f, y%f, z%f", entity, transform.pos.x, transform.pos.y, transform.pos.z);
//				const auto& sprite = registry.GetComponent<Sprite>(entity);
//				renderer->RenderSprite(sprite, transform.pos, transform.rot);
//			}
//			if (ImGui::TreeNode("Bodies")) {
//				for (auto que = registry.CreateQuery().Include<Transform, Body>(); const auto & entity : que.Find())
//				{
//
//					std::string s = ("Entity: "); s.append(std::to_string(entity));
//					if (ImGui::TreeNode(s.c_str()))
//					{
//						const auto& transform = registry.GetComponent<Transform>(entity);
//						ImGui::Text("Transform:\n Pos: x%f, y%f, z%f\n Rot: %f", transform.pos.x, transform.pos.y, transform.pos.z, transform.rot);
//
//						const auto& body = registry.GetComponent<Body>(entity);
//						ImGui::Text("\nBody:\n"
//							" Vel     : x%f, y%f, z%f\n"
//							" Accel   : x%f, y%f, z%f\n"
//							" RotVel  : %f\n"
//							" RotAccel: %f",
//							body.vel.x, body.vel.y, body.vel.z,
//							body.accel.x, body.accel.y, body.accel.z,
//							body.rotation,
//							body.angular);
//						if (ImGui::TreeNode("  Other Values"))
//						{
//							ImGui::Text("Max Speed: %f\n"
//								"  Max Acceleration: %f\n"
//								"  Max Rotation: %f\n"
//								"  Max Angular: %f\n"
//								"  Orientation: %f\n"
//								"  Mass: %f\n"
//								"  Radius: %f",
//								body.maxSpeed,
//								body.maxAcceleration,
//								body.maxRotation,
//								body.maxAngular,
//								body.orientation,
//								body.mass,
//								body.radius);
//							ImGui::TreePop();
//						}
//						ImGui::TreePop();
//					}
//				}
//				ImGui::TreePop();
//			}
//		}
//
//
//		ImGui::End();
//	}
//	renderer->EndFrame();
//}
//

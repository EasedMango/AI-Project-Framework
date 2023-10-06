#include "TestScene.h"
#include "ECS/ECS.h""
#include "Camera.h"
#include "Movement.h"
#include "MovementSystem.h"
#include "Player.h"
#include "PlayerMovementSystem.h"
#include "Transform.h"

bool TestScene::OnCreate(ECS& ecs, Ref<Window>& window)
{
	camera = std::make_shared<Camera>(window);
	Renderer::GetInstance().GiveCamera(camera);

	auto& registry = ecs.GetRegistry();
	auto& renderer = Renderer::GetInstance();


	auto headStick = registry.CreateEntity();
	registry.AddComponent<Transform>(headStick,glm::vec3(0,0,-1),0.f);
	registry.AddComponent<Movement>(headStick, glm::vec2(0, 0), 20.f);
	registry.AddComponent<Sprite>(headStick, renderer.CreateSprite("Head_With_A_Stick.png"));
	registry.AddComponent<Player>(headStick);


	auto headStick2 = registry.CreateEntity();
	registry.AddComponent<Transform>(headStick2, glm::vec3(-8, 1, 0), 0.f);
	registry.AddComponent<Movement>(headStick2, glm::vec2(1, 0), 0.5f);
	registry.AddComponent<Sprite>(headStick2, renderer.CreateSprite("Head_With_A_Stick.png",4));


	ecs.GetSystemManager().AddSystem<PlayerMovementSystem>();
	ecs.GetSystemManager().AddSystem<MovementSystem>();
	ecs.OnCreate();
	return true;
}

void TestScene::OnDestroy(ECS& ecs)
{
	ecs.OnDestroy();
}

void TestScene::Update(const float deltaTime, ECS& ecs)
{
	ecs.Update(deltaTime);
}

void TestScene::Render(ECS& ecs) const
{
	auto& registry = ecs.GetRegistry();
	auto& renderer = Renderer::GetInstance();
	
	renderer.BeginFrame();
	for(auto que = registry.CreateQuery().Include<Transform, Sprite>(); const auto& entity : que.Find()){
		auto& transform=registry.GetComponent<Transform>(entity);
		auto& sprite =registry.GetComponent<Sprite>(entity);

		renderer.RenderSprite(sprite, transform.position, transform.rotation);

	}
	renderer.EndFrame();
}

#include "PlayerMovementSystem.h"

#include <glm/geometric.hpp>

#include "InputHandler.h"
#include "Movement.h"
#include "Player.h"
#include "Transform.h"
#include "ECS/Registry.h"


void PlayerMovementSystem::OnCreate(Registry& registry)
{
	auto& input = InputHandler::Instance();


}

void PlayerMovementSystem::Update(const float& deltaTime, Registry& registry)
{
    static ID playerEntity = *registry.CreateQuery().Include<Player>().Find().begin();

	auto& input = InputHandler::Instance();
    float x = 0.0f, y = 0.0f;

    if (input.KeyHeld(SDLK_w)) {
        y += 1.0f;
    }
    if (input.KeyHeld(SDLK_s)) {
        y -= 1.0f;
    }
    if (input.KeyHeld(SDLK_a)) {
        x -= 1.0f;
    }
    if (input.KeyHeld(SDLK_d)) {
        x += 1.0f;
    }

    // Normalize individual axes to prevent them from exceeding 1 in magnitude
    x = std::clamp(x, -1.0f, 1.0f);
    y = std::clamp(y, -1.0f, 1.0f);

    // Assign the values to moveDir
    moveDir = glm::vec2(x, y);

    // If necessary, normalize moveDir if both x and y are non-zero
    if (x != 0.0f && y != 0.0f) {
        moveDir = glm::normalize(moveDir);
    }
    registry.GetComponent<Movement>(playerEntity).dir = moveDir;
}

void PlayerMovementSystem::OnDestroy(Registry& registry)
{
}

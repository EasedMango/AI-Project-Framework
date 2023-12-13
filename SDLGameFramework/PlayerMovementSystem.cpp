#include "PlayerMovementSystem.h"

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>


#include "Body.h"
#include "InputHandler.h"
#include "Player.h"
#include "Transform.h"
#include "ECS/Registry.h"


void PlayerMovementSystem::OnCreate(Registry& registry)
{
	auto& input = InputHandler::Instance();


}

void PlayerMovementSystem::Update(const float& deltaTime, Registry& registry)
{
    auto que = registry.CreateQuery().Include<Player>().Find();
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

    x = std::clamp(x, -1.0f, 1.0f);
    y = std::clamp(y, -1.0f, 1.0f);

    glm::vec2 moveDir = glm::vec2(x, y);

    if (x != 0.0f && y != 0.0f) {
        moveDir = glm::normalize(moveDir);
    }

    auto& body = registry.GetComponent<Body>(playerEntity);

    // Calculate desired velocity based on direction and max speed
    glm::vec2 desiredVel = moveDir * body.maxSpeed;

    // Calculate the acceleration needed to move the body to the desired velocity
    glm::vec2 steering = desiredVel - body.vel;

    // Optionally limit the acceleration
    if (glm::length(steering) > body.maxAcceleration) {
        steering = glm::normalize(steering) * body.maxAcceleration;
    }
    body.accel = steering;
    // Apply acceleration to the body
    float damping = 0.98f; // Closer to 1 means less damping
   // float damping = 1.0f - (1.0f - dampingFactor) * deltaTime;
    // Apply damping when no key is held
    if (x == 0.0f ) {
       
        body.vel.x *= damping;
    }

    if ( y == 0.0f) {
        body.vel.y *= damping;
    }

}


void PlayerMovementSystem::OnDestroy(Registry& registry)
{
}

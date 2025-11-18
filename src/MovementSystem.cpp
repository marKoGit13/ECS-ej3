#include "MovementSystem.h"
#include "World.h"
#include "TransformComponent.h"
#include "SpriteComponent.h" 
#include <algorithm> 

MovementSystem::MovementSystem(bool& isGameOverRef, int w, int h) 
    : screenWidth(w), screenHeight(h), mIsGameOver(isGameOverRef) {}

void MovementSystem::update(World& world, float dt) {
    if (mIsGameOver) return;

    for (auto& entity : world.getEntities()) {
        if (!entity->hasComponent<TransformComponent>()) {
            continue;
        }

        auto transform = entity->getComponent<TransformComponent>();
        transform->position += transform->velocity * dt;

        int w = 0, h = 0;
        if (entity->hasComponent<SpriteComponent>()) {
            auto sprite = entity->getComponent<SpriteComponent>();
            w = static_cast<int>(sprite->width * transform->scale.x);
            h = static_cast<int>(sprite->height * transform->scale.y);
        }

        transform->position.x = std::max(0.0f, transform->position.x);
        transform->position.x = std::min(transform->position.x, static_cast<float>(screenWidth - w));
        transform->position.y = std::max(0.0f, transform->position.y);
        transform->position.y = std::min(transform->position.y, static_cast<float>(screenHeight - h));
    }
}
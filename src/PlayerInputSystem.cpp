#include "PlayerInputSystem.h"
#include "World.h"
#include "TransformComponent.h"
#include "PlayerComponent.h"
#include <SDL2/SDL.h>

PlayerInputSystem::PlayerInputSystem(bool& isGameOverRef, float speed) 
    : playerSpeed(speed), mIsGameOver(isGameOverRef), mPlayerTransform(nullptr) {}

void PlayerInputSystem::update(World& world, float dt) {
    if (mPlayerTransform == nullptr) {
        for (auto& entity : world.getEntities()) {
            if (entity->hasComponent<PlayerComponent>()) {
                mPlayerTransform = entity->getComponent<TransformComponent>();
                break; 
            }
        }
    }

    if (mIsGameOver) {
        if (mPlayerTransform) {
            mPlayerTransform->velocity = {0.0f, 0.0f};
        }
        return; 
    }
    
    if (mPlayerTransform == nullptr) {
        return; 
    }

    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
    mPlayerTransform->velocity = {0.0f, 0.0f};

    if (keyboardState[SDL_SCANCODE_W]) mPlayerTransform->velocity.y = -playerSpeed;
    if (keyboardState[SDL_SCANCODE_S]) mPlayerTransform->velocity.y = playerSpeed;
    if (keyboardState[SDL_SCANCODE_A]) mPlayerTransform->velocity.x = -playerSpeed;
    if (keyboardState[SDL_SCANCODE_D]) mPlayerTransform->velocity.x = playerSpeed;
}
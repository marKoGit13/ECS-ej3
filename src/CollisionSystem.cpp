// Implementación de la lógica de detección y resolución
// busca pares colisionantes y ejecuta respuestas

#include "CollisionSystem.h"
#include "World.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"

CollisionSystem::CollisionSystem(bool& isGameOverRef)
    : mIsGameOver(isGameOverRef) {}

bool CollisionSystem::checkAABB(const SDL_Rect& a, const SDL_Rect& b) {
    if (a.x + a.w <= b.x || 
        b.x + b.w <= a.x || 
        a.y + a.h <= b.y || 
        b.y + b.h <= a.y) 
    {
        return false;
    }
    return true;
}

void CollisionSystem::update(World& world, float dt) {
    if (mIsGameOver) return;

    Entity* player = nullptr;
    TransformComponent* playerTransform = nullptr;
    ColliderComponent* playerCollider = nullptr;

    for (auto& entity : world.getEntities()) {
        if (entity->hasComponent<PlayerComponent>()) {
            player = entity.get();
            playerTransform = player->getComponent<TransformComponent>();
            playerCollider = player->getComponent<ColliderComponent>();
            break;
        }
    }

    if (player == nullptr || playerTransform == nullptr || playerCollider == nullptr) {
        return;
    }

    SDL_Rect playerHitbox = {
        // Posición + (Offset * Escala)
        static_cast<int>(playerTransform->position.x + (playerCollider->hitbox.x * playerTransform->scale.x)),
        static_cast<int>(playerTransform->position.y + (playerCollider->hitbox.y * playerTransform->scale.y)),
        // Ancho * Escala
        static_cast<int>(playerCollider->hitbox.w * playerTransform->scale.x),
        // Alto * Escala
        static_cast<int>(playerCollider->hitbox.h * playerTransform->scale.y)
    };

    for (auto& entity : world.getEntities()) {
        if (entity.get() == player || !entity->hasComponent<EnemyComponent>()) {
            continue;
        }
        
        if (entity->hasComponent<TransformComponent>() && entity->hasComponent<ColliderComponent>()) {
            auto enemyTransform = entity->getComponent<TransformComponent>();
            auto enemyCollider = entity->getComponent<ColliderComponent>();

            SDL_Rect enemyHitbox = {
                // Posición + (Offset * Escala)
                static_cast<int>(enemyTransform->position.x + (enemyCollider->hitbox.x * enemyTransform->scale.x)),
                static_cast<int>(enemyTransform->position.y + (enemyCollider->hitbox.y * enemyTransform->scale.y)),
                // Ancho * Escala
                static_cast<int>(enemyCollider->hitbox.w * enemyTransform->scale.x),
                // Alto * Escala
                static_cast<int>(enemyCollider->hitbox.h * enemyTransform->scale.y)
            };

            if (checkAABB(playerHitbox, enemyHitbox)) {
                world.emit(std::make_unique<DamageEvent>());
            }
        }
    }
}
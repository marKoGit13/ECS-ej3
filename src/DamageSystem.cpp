#include "DamageSystem.h"
#include "World.h"
#include "PlayerComponent.h"
#include "HealthComponent.h"
#include "spdlog/spdlog.h"

DamageSystem::DamageSystem(bool& isGameOverRef) 
    : mIsGameOver(isGameOverRef) {}

void DamageSystem::update(World& world, float dt) {
    std::unique_ptr<Event> event;
    while (world.poll(event)) {
        
        if (event->getType() == EventType::Damage) {
            
            if (mIsGameOver) continue; 

            for (auto& entity : world.getEntities()) {
                if (entity->hasComponent<PlayerComponent>() && entity->hasComponent<HealthComponent>()) {
                    auto health = entity->getComponent<HealthComponent>();

                    if (health->isInvincible()) {
                        spdlog::info("DamageSystem: Jugador golpeado, pero invencible.");
                        continue; 
                    }

                    spdlog::info("DamageSystem: Jugador ha recibido 1 de daño!");
                    health->hp -= 1;
                    health->invincibilityTimer = 1.5f; 

                    if (health->hp <= 0) {
                        spdlog::info("DamageSystem: HP del jugador a 0. GAME OVER.");
                        mIsGameOver = true; 
                    }
                    
                    break; 
                }
            }
        }
    }
}
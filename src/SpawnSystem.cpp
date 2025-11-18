#include "SpawnSystem.h"
#include "World.h"
#include "TransformComponent.h"
#include "EnemyComponent.h"
#include "HealthComponent.h"
#include "ColliderComponent.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include "spdlog/spdlog.h"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp> // para glm::length2
#include <fstream>
#include <string>
#include <random>

SpawnSystem::SpawnSystem(bool& isGameOverRef, SDL_Texture* enemyTexture, int screenWidth, int screenHeight, int& enemiesSpawned)
    : mIsGameOver(isGameOverRef),
      mEnemyTexture(enemyTexture),
      mSpawnTimer(0.0f), 
      mSpawnInterval(5.0f), // Default
      mScreenWidth(screenWidth), 
      mScreenHeight(screenHeight),
      mEnemiesSpawned(enemiesSpawned) 
{
    std::ifstream f("assets/data.json"); 
    if (f.is_open()) {
        std::string line;
        bool found = false;
        while (std::getline(f, line)) {
            std::size_t pos = line.find("\"spawnInterval\"");
            if (pos != std::string::npos) {
                std::size_t colonPos = line.find(':', pos);
                std::size_t valuePos = line.find_first_of("0123456789", colonPos);
                if (valuePos != std::string::npos) {
                    mSpawnInterval = std::stof(line.substr(valuePos));
                    spdlog::info("Spawn interval cargado de data.json: {}s", mSpawnInterval);
                    found = true;
                    break;
                }
            }
        }
        f.close();
        if (!found) {
            spdlog::warn("No se encontró 'spawnInterval' en data.json. Usando valor por defecto.");
        }
    } else {
        spdlog::warn("No se pudo abrir assets/data.json. Usando valor por defecto de 5.0s");
    }
}


void SpawnSystem::update(World& world, float dt) {
    if (mIsGameOver) return;
    
    // Logica de Persecución
    glm::vec2 playerPos = {mScreenWidth / 2.0f, mScreenHeight / 2.0f};
    for (auto& entity : world.getEntities()) {
        if (entity->hasComponent<PlayerComponent>() && entity->hasComponent<TransformComponent>()) {
            playerPos = entity->getComponent<TransformComponent>()->position;
            break;
        }
    }

    for (auto& entity : world.getEntities()) {
        if (entity->hasComponent<EnemyComponent>() && entity->hasComponent<TransformComponent>()) {
            auto transform = entity->getComponent<TransformComponent>();
            auto enemy = entity->getComponent<EnemyComponent>();
            glm::vec2 direction = playerPos - transform->position;
            
            if (glm::length2(direction) > 0.001f) { 
                direction = glm::normalize(direction);
            }
            transform->velocity = direction * enemy->speed;
        }
    }

    // Lógica de Spawning
    mSpawnTimer += dt;
    if (mSpawnTimer >= mSpawnInterval) {
        mSpawnTimer = 0.0f;
        spawnEnemy(world);
    }
}

void SpawnSystem::spawnEnemy(World& world) {
    spdlog::info("SpawnSystem: ¡Generando un nuevo enemigo!");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> edge(0, 3);
    std::uniform_real_distribution<> distW(0.0, mScreenWidth);
    std::uniform_real_distribution<> distH(0.0, mScreenHeight);
    glm::vec2 spawnPos;
    
    float spriteWidth = 32.0f; 
    float spriteHeight = 32.0f;
    if(mEnemyTexture) {
        int w, h;
        SDL_QueryTexture(mEnemyTexture, nullptr, nullptr, &w, &h);
        spriteWidth = static_cast<float>(w);
        spriteHeight = static_cast<float>(h);
    }

    int edgeCase = edge(gen);
    switch (edgeCase) {
        case 0: spawnPos = { 0.0f, distH(gen) }; break; // Izquierda
        case 1: spawnPos = { mScreenWidth - spriteWidth, distH(gen) }; break; // Derecha
        case 2: spawnPos = { distW(gen), 0.0f }; break; // Arriba
        default: spawnPos = { distW(gen), mScreenHeight - spriteHeight }; break; // Abajo
    }

    auto& enemy = world.createEntity();
    enemy.addComponent<TransformComponent>(
        spawnPos,         // Posición
        glm::vec2(0.0f),  // Velocidad 
        glm::vec2(0.1f)   // Escala 
    );
    enemy.addComponent<HealthComponent>(1); 
    
    std::uniform_real_distribution<> speedDist(50.0, 150.0);
    enemy.addComponent<EnemyComponent>(static_cast<float>(speedDist(gen)));
    
    if (mEnemyTexture) {
        enemy.addComponent<SpriteComponent>(mEnemyTexture, (int)spriteWidth, (int)spriteHeight);
        enemy.addComponent<ColliderComponent>(0, 0, (int)spriteWidth, (int)spriteHeight); 
    } else {
        spdlog::warn("Enemigo creado SIN textura/colisionador.");
    }
    mEnemiesSpawned++;
}
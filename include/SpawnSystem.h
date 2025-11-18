#pragma once
#include "ISystem.h"
#include <SDL2/SDL.h>

class SpawnSystem : public ISystem {
public:
    SpawnSystem(bool& isGameOverRef, SDL_Texture* enemyTexture, int screenWidth, int screenHeight, int& enemiesSpawned);
    void update(World& world, float dt) override;

private:
    void spawnEnemy(World& world);

    bool& mIsGameOver;
    SDL_Texture* mEnemyTexture;
    float mSpawnTimer;      
    float mSpawnInterval;   
    int mScreenWidth;
    int mScreenHeight;
    int& mEnemiesSpawned;
};
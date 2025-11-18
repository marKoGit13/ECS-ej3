#pragma once
#include "ISystem.h"
#include <SDL2/SDL.h>

class CollisionSystem : public ISystem {
public:
    CollisionSystem(bool& isGameOverRef);
    void update(World& world, float dt) override;

private:
    bool checkAABB(const SDL_Rect& a, const SDL_Rect& b);
    bool& mIsGameOver;
};
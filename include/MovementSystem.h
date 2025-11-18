#pragma once
#include "ISystem.h"

class MovementSystem : public ISystem {
public:
    int screenWidth;
    int screenHeight;
    MovementSystem(bool& isGameOverRef, int w, int h);
    void update(World& world, float dt) override;
private:
    bool& mIsGameOver;
};
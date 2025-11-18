#pragma once
#include "ISystem.h"
#include <glm/vec2.hpp>

class TransformComponent;

class PlayerInputSystem : public ISystem {
public:
    float playerSpeed; 
    PlayerInputSystem(bool& isGameOverRef, float speed = 200.0f);
    void update(World& world, float dt) override;
private:
    bool& mIsGameOver;
    TransformComponent* mPlayerTransform = nullptr;
};
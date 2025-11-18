// Captura y procesa la entrada del jugador
// traduce eventos de entrada en acciones de juego

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
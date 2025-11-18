#pragma once
#include "Component.h"
#include <glm/vec2.hpp> // Usamos GLM para vectores 2D

struct TransformComponent : public Component {
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 scale;

    TransformComponent(glm::vec2 pos = {0.0f, 0.0f}, glm::vec2 vel = {0.0f, 0.0f}, glm::vec2 sc = {1.0f, 1.0f})
        : position(pos), velocity(vel), scale(sc) {}
};  
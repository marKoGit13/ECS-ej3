// Marca una entidad como enemigo y guarda sus parámetros
// usado por sistemas de spawn y comportamiento

#pragma once
#include "Component.h"

struct EnemyComponent : public Component {
    float speed; // Velocidad de persecución

    EnemyComponent(float s = 100.0f) : speed(s) {}
};
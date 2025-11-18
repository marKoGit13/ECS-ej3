// Componente que contiene datos de colisión de la entidad
// usado por el sistema de colisiones

#pragma once
#include "Component.h"

struct ColliderComponent : public Component {
    SDL_Rect hitbox; // El rectángulo de colisión, relativo a la posición de la entidad

    ColliderComponent(int xOffset, int yOffset, int w, int h)
        : hitbox({xOffset, yOffset, w, h}) {}
};
// Componente que guarda información de sprite y textura
// usado por el sistema de render para dibujar entidades

#pragma once
#include "Component.h"
#include <string>

struct SpriteComponent : public Component {
    SDL_Texture* texture = nullptr;
    SDL_Rect srcRect;
    int width, height; // Dimensiones originales de la textura

    SpriteComponent(SDL_Texture* tex, int w, int h, int x = 0, int y = 0)
        : texture(tex), width(w), height(h) {
        srcRect = {x, y, w, h};
    }
};
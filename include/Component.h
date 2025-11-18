#pragma once
#include <SDL2/SDL.h> // Incluimos SDL aquí para que esté disponible en todos los componentes

// Declaración anticipada
class Entity;

struct Component {
    // Puntero a la entidad dueña de este componente
    Entity* entity = nullptr; 
    
    // Destructor virtual para permitir herencia
    virtual ~Component() = default;

    // Métodos virtuales que las subclases pueden implementar
    virtual void update(float dt) {}
    virtual void render(SDL_Renderer* renderer) {}
};
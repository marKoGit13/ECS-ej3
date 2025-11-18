// Contenedor que administra entidades, sistemas y eventos
// coordina el flujo del juego y el procesamiento

#pragma once
#include <vector>
#include <memory>       // Para std::unique_ptr
#include <queue>        // Para la cola de eventos
#include <SDL2/SDL.h>
#include "Entity.h"
#include "Event.h"
#include "ISystem.h"

class World {
public:
    World() = default;
    ~World() = default; // Los smart pointers se limpian solos

    // Deshabilitamos copia y asignación
    World(const World&) = delete;
    World& operator=(const World&) = delete;

    // Funciones principales del bucle
    void update(float dt);
    void render(SDL_Renderer* renderer);
    void refresh();
    Entity& createEntity(); 
    void emit(std::unique_ptr<Event> event);
    bool poll(std::unique_ptr<Event>& outEvent);
    void addSystem(std::unique_ptr<ISystem> system);
    std::vector<std::unique_ptr<Entity>>& getEntities();

private:
    // Vector de punteros inteligentes que posee todas las entidades
    std::vector<std::unique_ptr<Entity>> mEntities;
    
    // Cola para el bus de eventos
    std::queue<std::unique_ptr<Event>> mEventQueue; 

    // Lista de todos los sistemas
    std::vector<std::unique_ptr<ISystem>> mSystems; 
};
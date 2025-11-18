// Implementación del gestor de entidades, sistemas y eventos
// coordina actualizaciones y el flujo del juego

#include "World.h"
#include <algorithm> // Para std::remove_if
#include "spdlog/spdlog.h"


void World::update(float dt) {
    // Actualizar todos los sistemas (lógica del juego)
    for (auto& system : mSystems) {
        system->update(*this, dt);
    }

    // Actualizar todos los componentes de las entidades
    for (auto& entity : mEntities) {
        if (entity->isActive()) {
            entity->update(dt);
        }
    }
}


void World::render(SDL_Renderer* renderer) {
    for (auto& entity : mEntities) {
        if (entity->isActive()) {
            entity->render(renderer);
        }
    }
}


void World::refresh() {
    mEntities.erase(
        std::remove_if(mEntities.begin(), mEntities.end(),
            [](const std::unique_ptr<Entity>& e) { 
                return !e->isActive(); 
            }),
        mEntities.end()
    );
}

void World::addSystem(std::unique_ptr<ISystem> system) {
    mSystems.push_back(std::move(system));
    spdlog::info("System added");
}


Entity& World::createEntity() {
    auto entity = std::make_unique<Entity>(this);
    Entity& ref = *entity; 
    mEntities.push_back(std::move(entity));
    spdlog::info("Entity created");
    return ref; 
}


void World::emit(std::unique_ptr<Event> event) {
    mEventQueue.push(std::move(event)); 
}


bool World::poll(std::unique_ptr<Event>& outEvent) {
    if (mEventQueue.empty()) {
        return false; 
    }
    outEvent = std::move(mEventQueue.front()); 
    mEventQueue.pop();
    return true; 
}


std::vector<std::unique_ptr<Entity>>& World::getEntities() {
    return mEntities;
}
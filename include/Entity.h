#pragma once
#include <vector>
#include <memory>       // Para std::unique_ptr y std::make_unique
#include <map>          // Para búsqueda rápida de componentes
#include <typeindex>    // Para usar typeid como clave en el map
#include <utility>      // Para std::forward
#include "Component.h"
#include "spdlog/spdlog.h"

// Declaración anticipada
class World;

class Entity {
public:
    Entity(World* world) : mWorld(world) {}


    void update(float dt) {
        for (auto& comp : mComponents) {
            comp->update(dt);
        }
    }


    void render(SDL_Renderer* renderer) {
        for (auto& comp : mComponents) {
            comp->render(renderer);
        }
    }

    bool isActive() const { return mActive; }
    void destroy() { mActive = false; }
    World* getWorld() { return mWorld; }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... args) {
        // Creamos el componente usando smart pointers
        auto newComponent = std::make_unique<T>(std::forward<TArgs>(args)...);
        newComponent->entity = this; // Vinculamos el componente a esta entidad

        // Guardamos el puntero raw en el map para acceso rápido
        mComponentMap[std::type_index(typeid(T))] = newComponent.get();

        // Movemos la propiedad (ownership) del unique_ptr al vector
        mComponents.push_back(std::move(newComponent));
        
        return static_cast<T&>(*mComponents.back());
    }

    template <typename T>
    T* getComponent() {
        auto it = mComponentMap.find(std::type_index(typeid(T)));
        if (it != mComponentMap.end()) {
            return static_cast<T*>(it->second);
        }
        return nullptr;
    }

    template <typename T>
    bool hasComponent() const {
        return mComponentMap.count(std::type_index(typeid(T)));
    }

private:
    World* mWorld;
    bool mActive = true;

    // Vector de punteros inteligentes que posee todos los componentes
    std::vector<std::unique_ptr<Component>> mComponents;

    // Mapa de punteros raw para acceso rápido (no posee la memoria)
    std::map<std::type_index, Component*> mComponentMap;
};
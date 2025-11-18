// Componente que almacena la salud y gestión de vida
// expone métodos para recibir y curar daño

#pragma once
#include "Component.h"

struct HealthComponent : public Component {
    int hp;
    int maxHp;
    float invincibilityTimer;

    HealthComponent(int h = 3); // Solo dejamos la declaración

    // Declaramos el update
    void update(float dt) override; 

    bool isInvincible() const;
};
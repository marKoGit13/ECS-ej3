#include "HealthComponent.h"

HealthComponent::HealthComponent(int h)
    : hp(h), maxHp(h), invincibilityTimer(0.0f) {}

void HealthComponent::update(float dt) {
    if (invincibilityTimer > 0.0f) {
        invincibilityTimer -= dt;
        if (invincibilityTimer < 0.0f) {
            invincibilityTimer = 0.0f;
        }
    }
}

bool HealthComponent::isInvincible() const {
    return invincibilityTimer > 0.0f;
}
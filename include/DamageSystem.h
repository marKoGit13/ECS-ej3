// Sistema responsable de aplicar daño y actualizar salud
// procesa eventos de daño y muerte

#pragma once
#include "ISystem.h"
#include <memory>

class DamageSystem : public ISystem {
public:
    DamageSystem(bool& isGameOverRef);
    void update(World& world, float dt) override;
private:
    bool& mIsGameOver;
};
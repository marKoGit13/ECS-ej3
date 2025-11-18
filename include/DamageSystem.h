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
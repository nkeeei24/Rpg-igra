#pragma once

#include "core/Character.h"

namespace rpg {

class CombatSystem {
public:
    bool fight(Character& player, EnemyTemplate enemy);

private:
    int rollDamage(int sides, bool crit);
    void processStatusTick(CombatStats& stats, std::map<StatusEffect, int>& statuses, const std::string& owner);
};

} // namespace rpg

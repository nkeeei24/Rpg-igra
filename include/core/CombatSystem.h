#pragma once

#include "core/Character.h"

namespace rpg {

class CombatSystem {
public:
    bool fight(Character& player, EnemyTemplate enemy);

private:
    int rollDamage(int sides, bool crit);
    void processStatusTick(CombatStats& stats, std::map<StatusEffect, int>& statuses, const std::string& owner);
    bool useCombatItem(Character& player, std::map<StatusEffect, int>& enemyStatuses, CombatStats& enemyStats);
    bool useAbility(Character& player, std::map<StatusEffect, int>& enemyStatuses, CombatStats& enemyStats);
};

} // namespace rpg

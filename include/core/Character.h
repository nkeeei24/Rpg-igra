#pragma once

#include "core/Inventory.h"
#include "core/Types.h"

namespace rpg {

class Character {
public:
    Character();

    void setup(const std::string& name, Race race, ClassType cls);
    void gainXp(int amount);
    void rest();
    void applyStatus(StatusEffect status, int turns);
    bool hasStatus(StatusEffect status) const;
    void tickStatuses();

    std::string name;
    Race race{Race::Human};
    ClassType cls{ClassType::Warrior};
    int level{1};
    int xp{0};
    int xpToNext{100};
    int gold{25};
    int karma{0};
    CombatStats stats;
    Inventory inventory;
    std::map<Skill, int> skills;
    std::map<Faction, int> reputation;
    std::map<StatusEffect, int> activeStatuses;
    std::map<std::string, bool> worldFlags;
    std::vector<std::string> perks;

private:
    void choosePerk();
};

std::string toString(Race race);
std::string toString(ClassType cls);
std::string toString(Faction faction);
std::string toString(Skill skill);

} // namespace rpg

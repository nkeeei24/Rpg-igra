#include "core/SaveSystem.h"

#include <fstream>

namespace rpg {

bool SaveSystem::save(const Character& player, const std::string& locationId, int slot) const {
    std::ofstream out("save_slot_" + std::to_string(slot) + ".sav");
    if (!out) return false;
    out << player.name << "\n" << static_cast<int>(player.race) << " " << static_cast<int>(player.cls) << "\n";
    out << player.level << " " << player.xp << " " << player.xpToNext << " " << player.gold << " " << player.karma << "\n";
    out << player.stats.hp << " " << player.stats.maxHp << " " << player.stats.ac << " " << player.stats.attackBonus << " " << player.stats.damageDiceSides << " " << player.stats.speed << " " << player.stats.dodgeChance << "\n";
    out << locationId << "\n";
    return true;
}

bool SaveSystem::load(Character& player, std::string& locationId, int slot) const {
    std::ifstream in("save_slot_" + std::to_string(slot) + ".sav");
    if (!in) return false;
    std::getline(in, player.name);
    int r, c;
    in >> r >> c;
    player.race = static_cast<Race>(r);
    player.cls = static_cast<ClassType>(c);
    in >> player.level >> player.xp >> player.xpToNext >> player.gold >> player.karma;
    in >> player.stats.hp >> player.stats.maxHp >> player.stats.ac >> player.stats.attackBonus >> player.stats.damageDiceSides >> player.stats.speed >> player.stats.dodgeChance;
    in.ignore();
    std::getline(in, locationId);
    return true;
}

} // namespace rpg

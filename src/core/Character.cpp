#include "core/Character.h"

#include "systems/UI.h"

namespace rpg {

Character::Character() : inventory(60.0) {
    for (auto s : {Skill::Persuasion, Skill::Intimidation, Skill::Stealth, Skill::Lockpicking, Skill::Arcana, Skill::Survival}) {
        skills[s] = 1;
    }
    for (auto f : {Faction::Guild, Faction::Church, Faction::Bandits, Faction::Nobles}) {
        reputation[f] = 0;
    }
}

void Character::setup(const std::string& n, Race r, ClassType c) {
    name = n;
    race = r;
    cls = c;
    if (cls == ClassType::Warrior) {
        stats.maxHp = stats.hp = 130; stats.ac = 14; stats.attackBonus = 4; stats.damageDiceSides = 8;
    } else if (cls == ClassType::Rogue) {
        stats.maxHp = stats.hp = 105; stats.ac = 13; stats.attackBonus = 5; stats.speed = 14; skills[Skill::Stealth] = 4;
    } else if (cls == ClassType::Mage) {
        stats.maxHp = stats.hp = 90; stats.ac = 11; stats.attackBonus = 6; stats.damageDiceSides = 10; skills[Skill::Arcana] = 4;
    } else {
        stats.maxHp = stats.hp = 115; stats.ac = 13; stats.attackBonus = 4; skills[Skill::Persuasion] = 3;
    }
}

void Character::gainXp(int amount) {
    xp += amount;
    while (xp >= xpToNext && level < 20) {
        xp -= xpToNext;
        ++level;
        xpToNext += 60;
        stats.maxHp += 12;
        stats.hp = stats.maxHp;
        stats.attackBonus += 1;
        choosePerk();
        UI::printLog("Level up! You reached level " + std::to_string(level));
    }
}

void Character::choosePerk() {
    const int c = UI::chooseFromMenu("Choose perk:", {"Vitality (+20 max HP)", "Accuracy (+2 attack)", "Guardian (+1 AC)", "Silver Tongue (+2 Persuasion)"});
    if (c == 1) {
        stats.maxHp += 20; perks.push_back("Vitality");
    } else if (c == 2) {
        stats.attackBonus += 2; perks.push_back("Accuracy");
    } else if (c == 3) {
        stats.ac += 1; perks.push_back("Guardian");
    } else {
        skills[Skill::Persuasion] += 2; perks.push_back("Silver Tongue");
    }
}

void Character::rest() { stats.hp = std::min(stats.maxHp, stats.hp + 50); }

void Character::applyStatus(StatusEffect status, int turns) { activeStatuses[status] = turns; }

bool Character::hasStatus(StatusEffect status) const { return activeStatuses.count(status) && activeStatuses.at(status) > 0; }

void Character::tickStatuses() {
    for (auto it = activeStatuses.begin(); it != activeStatuses.end();) {
        it->second--;
        if (it->second <= 0) it = activeStatuses.erase(it);
        else ++it;
    }
}

std::string toString(Race race) {
    switch (race) { case Race::Human: return "Human"; case Race::Elf: return "Elf"; case Race::Dwarf: return "Dwarf"; default: return "Orc"; }
}

std::string toString(ClassType cls) {
    switch (cls) { case ClassType::Warrior: return "Warrior"; case ClassType::Rogue: return "Rogue"; case ClassType::Mage: return "Mage"; default: return "Cleric"; }
}

std::string toString(Faction faction) {
    switch (faction) { case Faction::Guild: return "Guild"; case Faction::Church: return "Church"; case Faction::Bandits: return "Bandits"; default: return "Nobles"; }
}

std::string toString(Skill skill) {
    switch (skill) {
        case Skill::Persuasion: return "Persuasion";
        case Skill::Intimidation: return "Intimidation";
        case Skill::Stealth: return "Stealth";
        case Skill::Lockpicking: return "Lockpicking";
        case Skill::Arcana: return "Arcana";
        default: return "Survival";
    }
}

} // namespace rpg

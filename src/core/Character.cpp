#include "core/Character.h"

#include <algorithm>

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

void Character::setupAbilities() {
    abilities.clear();
    if (cls == ClassType::Warrior) {
        abilities.push_back({"shield_bash", "Удар щитом", "Оглушает цель на 1 ход.", 3, 6, 0, 0, StatusEffect::Stun, 1});
        abilities.push_back({"battle_cry", "Боевой клич", "Увеличивает броню на 2.", 4, 0, 0, 2, StatusEffect::Buff, 2});
    } else if (cls == ClassType::Rogue) {
        abilities.push_back({"backstab", "Удар в спину", "Высокий урон и кровотечение.", 3, 10, 0, 0, StatusEffect::Bleed, 2});
        abilities.push_back({"smoke", "Дымовая шашка", "Повышает шанс уклонения.", 4, 0, 0, 0, StatusEffect::Buff, 2});
    } else if (cls == ClassType::Mage) {
        abilities.push_back({"firebolt", "Огненная стрела", "Наносит урон и поджигает.", 2, 12, 0, 0, StatusEffect::Burn, 2});
        abilities.push_back({"arcane_barrier", "Чародейский барьер", "Добавляет броню и снижает урон.", 4, 0, 0, 2, StatusEffect::Buff, 2});
    } else {
        abilities.push_back({"smite", "Священная кара", "Наносит урон и снимает яд с героя.", 3, 9, 0, 0, StatusEffect::Debuff, 0});
        abilities.push_back({"healing_light", "Исцеляющий свет", "Лечит героя.", 3, 0, 18, 0, StatusEffect::Debuff, 0});
    }
}

void Character::setup(const std::string& n, Race r, ClassType c) {
    name = n;
    race = r;
    cls = c;
    if (cls == ClassType::Warrior) {
        stats.maxHp = stats.hp = 130; stats.ac = 14; stats.attackBonus = 4; stats.damageDiceSides = 8;
    } else if (cls == ClassType::Rogue) {
        stats.maxHp = stats.hp = 105; stats.ac = 13; stats.attackBonus = 5; stats.speed = 14; stats.dodgeChance = 12; skills[Skill::Stealth] = 4;
    } else if (cls == ClassType::Mage) {
        stats.maxHp = stats.hp = 90; stats.ac = 11; stats.attackBonus = 6; stats.damageDiceSides = 10; skills[Skill::Arcana] = 4;
    } else {
        stats.maxHp = stats.hp = 115; stats.ac = 13; stats.attackBonus = 4; skills[Skill::Persuasion] = 3;
    }
    setupAbilities();
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
        UI::printLog("Новый уровень: " + std::to_string(level));
    }
}

void Character::choosePerk() {
    const int c = UI::chooseFromMenu("Выберите перк:", {"Живучесть (+20 HP)", "Точность (+2 атака)", "Страж (+1 AC)", "Дипломат (+2 Убеждение)"});
    if (c == 1) {
        stats.maxHp += 20; perks.push_back("Живучесть");
    } else if (c == 2) {
        stats.attackBonus += 2; perks.push_back("Точность");
    } else if (c == 3) {
        stats.ac += 1; perks.push_back("Страж");
    } else {
        skills[Skill::Persuasion] += 2; perks.push_back("Дипломат");
    }
}

void Character::rest() { stats.hp = std::min(stats.maxHp, stats.hp + 50); }
void Character::applyStatus(StatusEffect status, int turns) { activeStatuses[status] = std::max(activeStatuses[status], turns); }
bool Character::hasStatus(StatusEffect status) const { return activeStatuses.count(status) && activeStatuses.at(status) > 0; }

void Character::tickStatuses() {
    for (auto it = activeStatuses.begin(); it != activeStatuses.end();) {
        it->second--;
        if (it->second <= 0) it = activeStatuses.erase(it);
        else ++it;
    }
}

void Character::tickAbilityCooldowns() {
    for (auto& [id, cd] : abilityCooldowns) {
        if (cd > 0) --cd;
    }
}

bool Character::canUseAbility(const std::string& id) const {
    auto it = abilityCooldowns.find(id);
    return it == abilityCooldowns.end() || it->second <= 0;
}

void Character::triggerAbilityCooldown(const std::string& id) {
    for (const auto& ability : abilities) {
        if (ability.id == id) {
            abilityCooldowns[id] = ability.cooldown;
            return;
        }
    }
}

std::string toString(Race race) {
    switch (race) { case Race::Human: return "Человек"; case Race::Elf: return "Эльф"; case Race::Dwarf: return "Дварф"; default: return "Орк"; }
}

std::string toString(ClassType cls) {
    switch (cls) { case ClassType::Warrior: return "Воин"; case ClassType::Rogue: return "Разбойник"; case ClassType::Mage: return "Маг"; default: return "Клирик"; }
}

std::string toString(Faction faction) {
    switch (faction) { case Faction::Guild: return "Гильдия"; case Faction::Church: return "Церковь"; case Faction::Bandits: return "Бандиты"; default: return "Знать"; }
}

std::string toString(Skill skill) {
    switch (skill) {
        case Skill::Persuasion: return "Убеждение";
        case Skill::Intimidation: return "Запугивание";
        case Skill::Stealth: return "Скрытность";
        case Skill::Lockpicking: return "Взлом";
        case Skill::Arcana: return "Аркана";
        default: return "Выживание";
    }
}

} // namespace rpg

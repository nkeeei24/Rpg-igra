#pragma once

#include <map>
#include <string>
#include <vector>

namespace rpg {

enum class Race { Human, Elf, Dwarf, Orc };
enum class ClassType { Warrior, Rogue, Mage, Cleric };
enum class Skill { Persuasion, Intimidation, Stealth, Lockpicking, Arcana, Survival };
enum class Faction { Guild, Church, Bandits, Nobles };
enum class ItemType { Weapon, Armor, Ring, Potion, Scroll, Quest };
enum class Rarity { Common, Uncommon, Rare, Epic, Legendary };
enum class StatusEffect { Poison, Burn, Stun, Bleed, Buff, Debuff };
enum class QuestType { Main, Side, Random, Faction };
enum class LocationType { City, Forest, Ruins, Caves, Dungeon };

struct CombatStats {
    int hp{100};
    int maxHp{100};
    int ac{10};
    int attackBonus{2};
    int damageDiceSides{6};
    int speed{10};
    int dodgeChance{5};
};

struct Item {
    std::string id;
    std::string name;
    ItemType type{ItemType::Quest};
    Rarity rarity{Rarity::Common};
    int value{0};
    double weight{0.0};
    int power{0};
    bool consumable{false};
};

struct Ability {
    std::string id;
    std::string name;
    std::string description;
    int cooldown{0};
    int damage{0};
    int heal{0};
    int armorBuff{0};
    StatusEffect applyStatus{StatusEffect::Debuff};
    int statusTurns{0};
};

struct NPC {
    std::string id;
    std::string name;
    Faction faction{Faction::Guild};
    int attitude{0};
    std::vector<std::string> offeredQuests;
};

struct EnemyTemplate {
    std::string id;
    std::string name;
    CombatStats stats;
    int xpReward{10};
    int goldReward{5};
    std::vector<std::string> lootTable;
};

struct QuestStage {
    std::string description;
    bool completed{false};
};

struct Quest {
    std::string id;
    std::string title;
    QuestType type{QuestType::Side};
    std::vector<QuestStage> stages;
    bool completed{false};
    int karmaDelta{0};
    std::map<Faction, int> reputationDelta;
};

} // namespace rpg

#include "core/CombatSystem.h"

#include "systems/Dice.h"
#include "systems/UI.h"

namespace rpg {

int CombatSystem::rollDamage(int sides, bool crit) {
    const int base = Dice::roll(sides);
    return crit ? base * 2 : base;
}

void CombatSystem::processStatusTick(CombatStats& stats, std::map<StatusEffect, int>& statuses, const std::string& owner) {
    if (statuses[StatusEffect::Poison] > 0) {
        stats.hp -= 4; UI::printLog(owner + " suffers poison."); --statuses[StatusEffect::Poison];
    }
    if (statuses[StatusEffect::Burn] > 0) {
        stats.hp -= 5; UI::printLog(owner + " burns."); --statuses[StatusEffect::Burn];
    }
    if (statuses[StatusEffect::Bleed] > 0) {
        stats.hp -= 3; UI::printLog(owner + " bleeds."); --statuses[StatusEffect::Bleed];
    }
}

bool CombatSystem::fight(Character& player, EnemyTemplate enemy) {
    UI::printTitle("Battle: " + enemy.name);
    std::map<StatusEffect, int> enemyStatuses;
    const int pInit = Dice::roll(20) + player.stats.speed;
    const int eInit = Dice::roll(20) + enemy.stats.speed;
    bool playerTurn = pInit >= eInit;

    while (player.stats.hp > 0 && enemy.stats.hp > 0) {
        processStatusTick(player.stats, player.activeStatuses, player.name);
        processStatusTick(enemy.stats, enemyStatuses, enemy.name);
        if (player.stats.hp <= 0 || enemy.stats.hp <= 0) break;

        if (playerTurn) {
            if (player.hasStatus(StatusEffect::Stun)) {
                UI::printLog("You are stunned and lose a turn.");
                player.activeStatuses[StatusEffect::Stun]--;
            } else {
                const int roll = Dice::rollD20();
                if (roll == 1) UI::printLog("Critical fail! You slip and miss.");
                else {
                    const bool crit = roll == 20;
                    const int hit = roll + player.stats.attackBonus;
                    if (hit >= enemy.stats.ac || crit) {
                        int dmg = rollDamage(player.stats.damageDiceSides, crit);
                        if (enemyStatuses[StatusEffect::Debuff] > 0) dmg += 2;
                        enemy.stats.hp -= dmg;
                        UI::printLog("You hit for " + std::to_string(dmg));
                        if (Dice::roll(100) <= 18) enemyStatuses[StatusEffect::Bleed] = 2;
                    } else UI::printLog("You miss.");
                }
            }
        } else {
            const int roll = Dice::rollD20();
            const bool crit = roll == 20;
            if (roll == 1) {
                UI::printLog(enemy.name + " critically fails.");
            } else {
                const int evadeRoll = Dice::roll(100);
                if (evadeRoll <= player.stats.dodgeChance) {
                    UI::printLog("You dodge incoming hit.");
                } else if (roll + enemy.stats.attackBonus >= player.stats.ac || crit) {
                    int dmg = rollDamage(enemy.stats.damageDiceSides, crit);
                    if (player.hasStatus(StatusEffect::Buff)) dmg = std::max(1, dmg - 2);
                    player.stats.hp -= dmg;
                    UI::printLog(enemy.name + " hits for " + std::to_string(dmg));
                    if (Dice::roll(100) <= 15) player.applyStatus(StatusEffect::Poison, 2);
                    if (Dice::roll(100) <= 10) player.applyStatus(StatusEffect::Stun, 1);
                }
            }
        }

        UI::printHint("HP: " + std::to_string(player.stats.hp) + "/" + std::to_string(player.stats.maxHp) +
                      " | " + enemy.name + " HP: " + std::to_string(std::max(0, enemy.stats.hp)));
        playerTurn = !playerTurn;
    }

    if (player.stats.hp <= 0) return false;
    player.gold += enemy.goldReward;
    player.gainXp(enemy.xpReward);
    return true;
}

} // namespace rpg

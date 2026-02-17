#include "core/CombatSystem.h"

#include <algorithm>

#include "systems/Dice.h"
#include "systems/UI.h"

namespace rpg {

int CombatSystem::rollDamage(int sides, bool crit) {
    const int base = Dice::roll(sides);
    return crit ? base * 2 : base;
}

void CombatSystem::processStatusTick(CombatStats& stats, std::map<StatusEffect, int>& statuses, const std::string& owner) {
    if (statuses[StatusEffect::Poison] > 0) {
        stats.hp -= 4; UI::printLog(owner + " получает урон от яда."); --statuses[StatusEffect::Poison];
    }
    if (statuses[StatusEffect::Burn] > 0) {
        stats.hp -= 5; UI::printLog(owner + " горит."); --statuses[StatusEffect::Burn];
    }
    if (statuses[StatusEffect::Bleed] > 0) {
        stats.hp -= 3; UI::printLog(owner + " истекает кровью."); --statuses[StatusEffect::Bleed];
    }
}

bool CombatSystem::useCombatItem(Character& player, std::map<StatusEffect, int>& enemyStatuses, CombatStats& enemyStats) {
    auto& inv = player.inventory.itemsMutable();
    for (size_t i = 0; i < inv.size(); ++i) {
        if (!inv[i].consumable) continue;
        const Item item = inv[i];
        UI::printLog("Вы используете: " + item.name);
        if (item.id == "health_potion" || item.id == "bandage") {
            player.stats.hp = std::min(player.stats.maxHp, player.stats.hp + item.power);
        } else if (item.id == "antidote") {
            player.activeStatuses[StatusEffect::Poison] = 0;
        } else if (item.id == "fire_bomb") {
            enemyStats.hp -= item.power;
            enemyStatuses[StatusEffect::Burn] = 2;
        } else if (item.id == "scroll_frost") {
            enemyStatuses[StatusEffect::Stun] = 1;
        } else if (item.id == "elixir_guard") {
            player.stats.ac += 2;
            player.applyStatus(StatusEffect::Buff, 3);
        }
        inv.erase(inv.begin() + static_cast<long>(i));
        return true;
    }
    UI::printLog("Нет расходников для применения.");
    return false;
}

bool CombatSystem::useAbility(Character& player, std::map<StatusEffect, int>& enemyStatuses, CombatStats& enemyStats) {
    if (player.abilities.empty()) return false;
    std::vector<std::string> options;
    for (const auto& a : player.abilities) {
        const int cd = player.abilityCooldowns[a.id];
        options.push_back(a.name + " (КД: " + std::to_string(cd) + ")");
    }
    options.push_back("Отмена");
    int choice = UI::chooseFromMenu("Способности:", options);
    if (choice < 1 || choice > static_cast<int>(player.abilities.size())) return false;
    auto ability = player.abilities[choice - 1];
    if (!player.canUseAbility(ability.id)) {
        UI::printLog("Способность на перезарядке.");
        return false;
    }

    if (ability.damage > 0) enemyStats.hp -= ability.damage + Dice::roll(6);
    if (ability.heal > 0) player.stats.hp = std::min(player.stats.maxHp, player.stats.hp + ability.heal);
    if (ability.armorBuff > 0) player.stats.ac += ability.armorBuff;
    if (ability.statusTurns > 0) enemyStatuses[ability.applyStatus] = ability.statusTurns;
    if (ability.id == "smite") player.activeStatuses[StatusEffect::Poison] = 0;
    if (ability.id == "smoke") player.stats.dodgeChance += 10;

    player.triggerAbilityCooldown(ability.id);
    UI::printLog("Применена способность: " + ability.name);
    return true;
}

bool CombatSystem::fight(Character& player, EnemyTemplate enemy) {
    UI::printTitle("Бой: " + enemy.name);
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
                UI::printLog("Вы оглушены и пропускаете ход.");
                player.activeStatuses[StatusEffect::Stun]--;
            } else {
                int action = UI::chooseFromMenu("Ваш ход:", {"Атака", "Способность", "Предмет", "Защита"});
                if (action == 2) {
                    useAbility(player, enemyStatuses, enemy.stats);
                } else if (action == 3) {
                    useCombatItem(player, enemyStatuses, enemy.stats);
                } else if (action == 4) {
                    player.applyStatus(StatusEffect::Buff, 1);
                    UI::printLog("Вы принимаете защитную стойку.");
                } else {
                    const int roll = Dice::rollD20();
                    if (roll == 1) UI::printLog("Критический провал! Атака сорвана.");
                    else {
                        const bool crit = roll == 20;
                        const int hit = roll + player.stats.attackBonus;
                        if (hit >= enemy.stats.ac || crit) {
                            int dmg = rollDamage(player.stats.damageDiceSides, crit);
                            if (enemyStatuses[StatusEffect::Debuff] > 0) dmg += 2;
                            enemy.stats.hp -= dmg;
                            UI::printLog("Вы наносите урон: " + std::to_string(dmg));
                            if (Dice::roll(100) <= 18) enemyStatuses[StatusEffect::Bleed] = 2;
                        } else UI::printLog("Промах.");
                    }
                }
            }
        } else {
            if (enemyStatuses[StatusEffect::Stun] > 0) {
                UI::printLog(enemy.name + " оглушен и пропускает ход.");
                enemyStatuses[StatusEffect::Stun]--;
            } else {
                const int roll = Dice::rollD20();
                const bool crit = roll == 20;
                if (roll == 1) {
                    UI::printLog(enemy.name + " критически промахивается.");
                } else {
                    const int evadeRoll = Dice::roll(100);
                    if (evadeRoll <= player.stats.dodgeChance) {
                        UI::printLog("Вы уклоняетесь от удара.");
                    } else if (roll + enemy.stats.attackBonus >= player.stats.ac || crit) {
                        int dmg = rollDamage(enemy.stats.damageDiceSides, crit);
                        if (player.hasStatus(StatusEffect::Buff)) dmg = std::max(1, dmg - 2);
                        player.stats.hp -= dmg;
                        UI::printLog(enemy.name + " наносит урон: " + std::to_string(dmg));
                        if (Dice::roll(100) <= 15) player.applyStatus(StatusEffect::Poison, 2);
                        if (Dice::roll(100) <= 10) player.applyStatus(StatusEffect::Stun, 1);
                    }
                }
            }
        }

        UI::printHint("HP: " + std::to_string(player.stats.hp) + "/" + std::to_string(player.stats.maxHp) +
                      " | " + enemy.name + " HP: " + std::to_string(std::max(0, enemy.stats.hp)));
        playerTurn = !playerTurn;
        player.tickAbilityCooldowns();
    }

    if (player.stats.hp <= 0) return false;
    player.gold += enemy.goldReward;
    player.gainXp(enemy.xpReward);
    return true;
}

} // namespace rpg

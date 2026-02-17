#include "core/DialogueSystem.h"

#include "systems/Dice.h"
#include "systems/UI.h"

namespace rpg {

bool DialogueSystem::skillCheck(Character& player, Skill skill, int dc, bool hidden) {
    const int roll = Dice::rollD20();
    const int total = roll + player.skills[skill];
    if (!hidden) {
        UI::printLog("Проверка " + toString(skill) + ": " + std::to_string(roll) + " + " + std::to_string(player.skills[skill]) + " против СЛ " + std::to_string(dc));
    }
    return total >= dc;
}

bool DialogueSystem::runConversation(Character& player, NPC& npc) {
    UI::printTitle("Разговор: " + npc.name);
    int c = UI::chooseFromMenu("Выберите подход:", {"Дипломатия (Убеждение)", "Угроза (Запугивание)", "Наблюдение (скрытая Скрытность)", "Арканный анализ (Аркана)", "Уйти"});
    if (c == 5) return false;

    bool ok = false;
    if (c == 1) ok = skillCheck(player, Skill::Persuasion, 12);
    if (c == 2) ok = skillCheck(player, Skill::Intimidation, 13);
    if (c == 3) ok = skillCheck(player, Skill::Stealth, 11, true);
    if (c == 4) ok = skillCheck(player, Skill::Arcana, 14);

    if (ok) {
        npc.attitude += 1;
        player.reputation[npc.faction] += 1;
        UI::printLog(npc.name + " настроен дружелюбнее. Открыты новые возможности.");
    } else {
        npc.attitude -= 1;
        UI::printLog("Диалог провален, отношение ухудшилось.");
    }
    return ok;
}

} // namespace rpg

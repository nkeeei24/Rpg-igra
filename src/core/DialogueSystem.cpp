#include "core/DialogueSystem.h"

#include "systems/Dice.h"
#include "systems/UI.h"

namespace rpg {

bool DialogueSystem::skillCheck(Character& player, Skill skill, int dc, bool hidden) {
    const int roll = Dice::rollD20();
    const int total = roll + player.skills[skill];
    if (!hidden) {
        UI::printLog("Skill check " + toString(skill) + ": " + std::to_string(roll) + " + " + std::to_string(player.skills[skill]) + " vs DC " + std::to_string(dc));
    }
    return total >= dc;
}

bool DialogueSystem::runConversation(Character& player, NPC& npc) {
    UI::printTitle("Conversation with " + npc.name);
    int c = UI::chooseFromMenu("Choose approach:", {"Friendly talk (Persuasion)", "Threaten (Intimidation)", "Observe silently (hidden Stealth)", "Leave"});
    if (c == 4) return false;

    bool ok = false;
    if (c == 1) ok = skillCheck(player, Skill::Persuasion, 12);
    if (c == 2) ok = skillCheck(player, Skill::Intimidation, 13);
    if (c == 3) ok = skillCheck(player, Skill::Stealth, 11, true);

    if (ok) {
        npc.attitude += 1;
        player.reputation[npc.faction] += 1;
        UI::printLog(npc.name + " seems convinced. New opportunities may appear.");
    } else {
        npc.attitude -= 1;
        UI::printLog("The conversation failed, attitude worsened.");
    }
    return ok;
}

} // namespace rpg

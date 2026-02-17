#pragma once

#include "core/Character.h"

namespace rpg {

class DialogueSystem {
public:
    bool runConversation(Character& player, NPC& npc);

private:
    bool skillCheck(Character& player, Skill skill, int dc, bool hidden = false);
};

} // namespace rpg

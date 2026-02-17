#pragma once

#include "core/QuestSystem.h"
#include "core/World.h"

namespace rpg {

class ContentFactory {
public:
    static void populateWorld(World& world);
    static void populateQuests(QuestSystem& quests);
};

} // namespace rpg

#pragma once

#include "core/Character.h"
#include "core/QuestSystem.h"

namespace rpg {

class SaveSystem {
public:
    bool save(const Character& player, const std::string& locationId, int slot) const;
    bool load(Character& player, std::string& locationId, int slot) const;
};

} // namespace rpg

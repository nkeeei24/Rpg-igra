#include "core/World.h"

namespace rpg {

void World::initialize() {}

const std::map<std::string, Location>& World::locations() const { return locations_; }
std::map<std::string, Location>& World::locationsMutable() { return locations_; }
std::map<std::string, NPC>& World::npcs() { return npcs_; }
std::map<std::string, EnemyTemplate>& World::enemies() { return enemies_; }
std::map<std::string, Item>& World::items() { return items_; }

} // namespace rpg

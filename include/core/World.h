#pragma once

#include "core/Types.h"

namespace rpg {

struct Location {
    std::string id;
    std::string name;
    LocationType type{LocationType::City};
    std::vector<std::string> connected;
    std::vector<std::string> npcs;
    std::vector<std::string> enemies;
};

class World {
public:
    void initialize();
    const std::map<std::string, Location>& locations() const;
    std::map<std::string, Location>& locationsMutable();
    std::map<std::string, NPC>& npcs();
    std::map<std::string, EnemyTemplate>& enemies();
    std::map<std::string, Item>& items();

private:
    std::map<std::string, Location> locations_;
    std::map<std::string, NPC> npcs_;
    std::map<std::string, EnemyTemplate> enemies_;
    std::map<std::string, Item> items_;
};

} // namespace rpg

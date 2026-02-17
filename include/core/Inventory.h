#pragma once

#include "core/Types.h"

namespace rpg {

class Inventory {
public:
    explicit Inventory(double maxWeight = 50.0);

    bool addItem(const Item& item);
    bool removeItem(const std::string& itemId);
    Item* findItem(const std::string& itemId);
    const std::vector<Item>& items() const;
    double currentWeight() const;
    double maxWeight() const;

private:
    std::vector<Item> items_;
    double maxWeight_{50.0};
};

} // namespace rpg

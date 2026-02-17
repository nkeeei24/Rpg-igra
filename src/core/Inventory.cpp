#include "core/Inventory.h"
#include <algorithm>

namespace rpg {

Inventory::Inventory(double maxWeight) : maxWeight_(maxWeight) {}

bool Inventory::addItem(const Item& item) {
    if (currentWeight() + item.weight > maxWeight_) return false;
    items_.push_back(item);
    return true;
}

bool Inventory::removeItem(const std::string& itemId) {
    auto it = std::find_if(items_.begin(), items_.end(), [&](const Item& i) { return i.id == itemId; });
    if (it == items_.end()) return false;
    items_.erase(it);
    return true;
}

Item* Inventory::findItem(const std::string& itemId) {
    auto it = std::find_if(items_.begin(), items_.end(), [&](const Item& i) { return i.id == itemId; });
    return it == items_.end() ? nullptr : &(*it);
}

const std::vector<Item>& Inventory::items() const { return items_; }

double Inventory::currentWeight() const {
    double total = 0.0;
    for (const auto& i : items_) total += i.weight;
    return total;
}

double Inventory::maxWeight() const { return maxWeight_; }

} // namespace rpg

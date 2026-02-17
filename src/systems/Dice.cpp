#include "systems/Dice.h"

namespace rpg {

std::mt19937& Dice::rng() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

int Dice::roll(int sides) {
    std::uniform_int_distribution<int> dist(1, sides);
    return dist(rng());
}

int Dice::rollD20(AdvantageType adv) {
    const int a = roll(20);
    if (adv == AdvantageType::Normal) {
        return a;
    }
    const int b = roll(20);
    return adv == AdvantageType::Advantage ? std::max(a, b) : std::min(a, b);
}

std::string Dice::toString(AdvantageType adv) {
    if (adv == AdvantageType::Advantage) return "advantage";
    if (adv == AdvantageType::Disadvantage) return "disadvantage";
    return "normal";
}

} // namespace rpg

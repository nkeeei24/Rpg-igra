#pragma once

#include <random>
#include <string>

namespace rpg {

enum class AdvantageType { Normal, Advantage, Disadvantage };

class Dice {
public:
    static int roll(int sides);
    static int rollD20(AdvantageType adv = AdvantageType::Normal);
    static std::string toString(AdvantageType adv);

private:
    static std::mt19937& rng();
};

} // namespace rpg

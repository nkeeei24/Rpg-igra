#pragma once

#include <string>
#include <vector>

namespace rpg {

class UI {
public:
    static void printTitle(const std::string& text);
    static void printDivider();
    static void printLog(const std::string& text);
    static void printHint(const std::string& text);
    static int chooseFromMenu(const std::string& prompt, const std::vector<std::string>& options);
    static std::string inputLine(const std::string& prompt);

    static constexpr const char* RESET = "\033[0m";
    static constexpr const char* GOLD = "\033[33m";
    static constexpr const char* GREEN = "\033[32m";
    static constexpr const char* RED = "\033[31m";
    static constexpr const char* CYAN = "\033[36m";
};

} // namespace rpg

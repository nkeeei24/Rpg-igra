#include "systems/UI.h"

#include <iostream>
#include <limits>

namespace rpg {

void UI::printTitle(const std::string& text) {
    std::cout << GOLD << "\n=== " << text << " ===" << RESET << "\n";
}

void UI::printDivider() {
    std::cout << CYAN << "------------------------------------------" << RESET << "\n";
}

void UI::printLog(const std::string& text) {
    std::cout << GREEN << "[LOG] " << RESET << text << "\n";
}

void UI::printHint(const std::string& text) {
    std::cout << CYAN << "[HINT] " << RESET << text << "\n";
}

int UI::chooseFromMenu(const std::string& prompt, const std::vector<std::string>& options) {
    std::cout << prompt << "\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << "  " << i + 1 << ") " << options[i] << "\n";
    }
    std::cout << "> ";
    int choice = 0;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (choice < 1 || choice > static_cast<int>(options.size())) return 1;
    return choice;
}

std::string UI::inputLine(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

} // namespace rpg

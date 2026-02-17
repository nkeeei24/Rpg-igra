#include "core/Game.h"

#include "content/ContentFactory.h"
#include "systems/Dice.h"
#include "systems/UI.h"

namespace rpg {

void Game::run() {
    ContentFactory::populateWorld(world_);
    ContentFactory::populateQuests(quests_);
    mainMenu();
}

void Game::mainMenu() {
    while (running_) {
        UI::printTitle("Legends of the Ashen Realm");
        int c = UI::chooseFromMenu("Main menu:", {"New game", "Load game", "Exit"});
        if (c == 1) newGame();
        else if (c == 2) {
            if (saves_.load(player_, currentLocation_, 1)) gameLoop();
            else UI::printLog("No save found in slot 1.");
        } else running_ = false;
    }
}

void Game::newGame() {
    const std::string name = UI::inputLine("Enter hero name: ");
    int r = UI::chooseFromMenu("Choose race:", {"Human", "Elf", "Dwarf", "Orc"});
    int c = UI::chooseFromMenu("Choose class:", {"Warrior", "Rogue", "Mage", "Cleric"});
    player_ = Character();
    player_.setup(name, static_cast<Race>(r - 1), static_cast<ClassType>(c - 1));
    quests_.acceptQuest("q_main_1");
    gameLoop();
}

void Game::showPlayerSheet() const {
    UI::printTitle("Character");
    UI::printLog(player_.name + " | " + toString(player_.race) + " " + toString(player_.cls));
    UI::printLog("Level " + std::to_string(player_.level) + " XP " + std::to_string(player_.xp) + "/" + std::to_string(player_.xpToNext));
    UI::printLog("HP " + std::to_string(player_.stats.hp) + "/" + std::to_string(player_.stats.maxHp) + " AC " + std::to_string(player_.stats.ac));
    UI::printLog("Gold: " + std::to_string(player_.gold) + " Karma: " + std::to_string(player_.karma));
}

void Game::explore() {
    const auto& loc = world_.locations().at(currentLocation_);
    std::vector<std::string> opts;
    for (const auto& n : loc.connected) opts.push_back("Travel to " + world_.locations().at(n).name);
    opts.push_back("Stay");
    int c = UI::chooseFromMenu("Current location: " + loc.name, opts);
    if (c <= static_cast<int>(loc.connected.size())) {
        currentLocation_ = loc.connected[c - 1];
        UI::printLog("You travel to " + world_.locations().at(currentLocation_).name);
        randomEvent();
    }
}

void Game::randomEvent() {
    int r = Dice::roll(100);
    if (r < 35) {
        const auto& enemies = world_.locations().at(currentLocation_).enemies;
        if (!enemies.empty()) {
            auto e = world_.enemies().at(enemies[Dice::roll(static_cast<int>(enemies.size())) - 1]);
            if (!combat_.fight(player_, e)) {
                UI::printLog("You were defeated... rescued back to Eldenport with losses.");
                player_.gold = std::max(0, player_.gold - 20);
                player_.stats.hp = player_.stats.maxHp / 2;
                currentLocation_ = "eldenport";
            }
        }
    } else if (r < 55) {
        UI::printLog("You find a stash with supplies.");
        player_.gold += 15;
        player_.inventory.addItem(world_.items().at("health_potion"));
    } else if (r < 70) {
        UI::printLog("An ambush! You escape but get scratched.");
        player_.stats.hp -= 8;
    } else {
        UI::printHint("Quiet roads. Nothing happened.");
    }
}

void Game::interactNPC() {
    auto& npcs = world_.locations().at(currentLocation_).npcs;
    if (npcs.empty()) { UI::printLog("No one to talk to here."); return; }
    std::vector<std::string> opts;
    for (const auto& id : npcs) opts.push_back(world_.npcs().at(id).name);
    int c = UI::chooseFromMenu("Choose NPC:", opts);
    auto& npc = world_.npcs().at(npcs[c - 1]);
    bool success = dialogue_.runConversation(player_, npc);
    if (success && !npc.offeredQuests.empty()) {
        quests_.acceptQuest(npc.offeredQuests.front());
        UI::printLog("Quest accepted: " + npc.offeredQuests.front());
    }
}

void Game::visitShop() {
    UI::printTitle("Shop and repairs");
    int c = UI::chooseFromMenu("Choose action:", {"Buy health potion (12)", "Sell first item (+8)", "Repair gear (20)", "Leave"});
    if (c == 1 && player_.gold >= 12) {
        player_.gold -= 12;
        player_.inventory.addItem(world_.items().at("health_potion"));
    } else if (c == 2 && !player_.inventory.items().empty()) {
        player_.gold += 8;
        player_.inventory.removeItem(player_.inventory.items().front().id);
    } else if (c == 3 && player_.gold >= 20) {
        player_.gold -= 20;
        player_.stats.ac += 1;
    }
}

void Game::rest() { player_.rest(); UI::printLog("You rest at camp and recover HP."); }

void Game::gameLoop() {
    campaignFinished_ = false;
    while (!campaignFinished_ && running_) {
        if (player_.stats.hp <= 0) break;
        UI::printDivider();
        int c = UI::chooseFromMenu("Action:", {"Explore/Travel", "Talk", "Fight local enemy", "Shop", "Rest", "Quest Journal", "Character", "Save", "End game"});
        if (c == 1) explore();
        else if (c == 2) interactNPC();
        else if (c == 3) {
            const auto& enemies = world_.locations().at(currentLocation_).enemies;
            if (!enemies.empty()) {
                auto e = world_.enemies().at(enemies[Dice::roll(static_cast<int>(enemies.size())) - 1]);
                bool win = combat_.fight(player_, e);
                if (!win) { player_.stats.hp = player_.stats.maxHp / 2; currentLocation_ = "eldenport"; }
                else if (e.id == "boss") {
                    quests_.advanceQuest("q_main_3", player_);
                    campaignFinished_ = true;
                }
            }
        } else if (c == 4) visitShop();
        else if (c == 5) rest();
        else if (c == 6) quests_.showJournal();
        else if (c == 7) showPlayerSheet();
        else if (c == 8) {
            if (saves_.save(player_, currentLocation_, 1)) UI::printLog("Saved in slot 1");
        } else {
            running_ = false;
        }

        if (player_.level >= 4 && quests_.isCompleted("q_main_1")) quests_.acceptQuest("q_main_2");
        if (player_.level >= 7 && quests_.isCompleted("q_main_2")) quests_.acceptQuest("q_main_3");
        if (currentLocation_ == "silverwood") quests_.advanceQuest("q_main_1", player_);
        if (currentLocation_ == "ruins") quests_.advanceQuest("q_main_2", player_);

        if (campaignFinished_) {
            UI::printTitle("Finale");
            if (player_.karma >= 2) UI::printLog("You restore hope to the realm. The Church and Guild hail you as a savior.");
            else if (player_.reputation[Faction::Bandits] > 2) UI::printLog("You claim the Tyrant Crown and rule through fear alongside bandits.");
            else UI::printLog("The realm survives, but your methods remain controversial among factions.");
        }
    }
}

} // namespace rpg

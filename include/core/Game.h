#pragma once

#include "core/CombatSystem.h"
#include "core/DialogueSystem.h"
#include "core/QuestSystem.h"
#include "core/SaveSystem.h"
#include "core/World.h"

namespace rpg {

class Game {
public:
    void run();

private:
    void mainMenu();
    void newGame();
    void gameLoop();
    void showPlayerSheet() const;
    void explore();
    void randomEvent();
    void interactNPC();
    void visitShop();
    void rest();
    void useInventoryOutsideCombat();

    bool running_{true};
    bool campaignFinished_{false};
    Character player_;
    World world_;
    QuestSystem quests_;
    DialogueSystem dialogue_;
    CombatSystem combat_;
    SaveSystem saves_;
    std::string currentLocation_ = "gorod_svetograd";
};

} // namespace rpg

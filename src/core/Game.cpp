#include "core/Game.h"

#include <algorithm>

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
        UI::printTitle("Легенды Пепельного Королевства");
        int c = UI::chooseFromMenu("Главное меню:", {"Новая игра", "Загрузить", "Выход"});
        if (c == 1) newGame();
        else if (c == 2) {
            if (saves_.load(player_, currentLocation_, 1)) gameLoop();
            else UI::printLog("Сохранение в слоте 1 не найдено.");
        } else running_ = false;
    }
}

void Game::newGame() {
    const std::string name = UI::inputLine("Введите имя героя: ");
    int r = UI::chooseFromMenu("Выберите расу:", {"Человек", "Эльф", "Дварф", "Орк"});
    int c = UI::chooseFromMenu("Выберите класс:", {"Воин", "Разбойник", "Маг", "Клирик"});
    player_ = Character();
    player_.setup(name, static_cast<Race>(r - 1), static_cast<ClassType>(c - 1));
    player_.inventory.addItem(world_.items().at("health_potion"));
    player_.inventory.addItem(world_.items().at("bandage"));
    quests_.acceptQuest("q_main_1");
    gameLoop();
}

void Game::showPlayerSheet() const {
    UI::printTitle("Лист персонажа");
    UI::printLog(player_.name + " | " + toString(player_.race) + " " + toString(player_.cls));
    UI::printLog("Уровень " + std::to_string(player_.level) + " XP " + std::to_string(player_.xp) + "/" + std::to_string(player_.xpToNext));
    UI::printLog("HP " + std::to_string(player_.stats.hp) + "/" + std::to_string(player_.stats.maxHp) + " AC " + std::to_string(player_.stats.ac));
    UI::printLog("Золото: " + std::to_string(player_.gold) + " | Карма: " + std::to_string(player_.karma));
    UI::printLog("Инвентарь: " + std::to_string(player_.inventory.items().size()) + " предметов, вес " + std::to_string(player_.inventory.currentWeight()));
}

void Game::explore() {
    const auto& loc = world_.locations().at(currentLocation_);
    std::vector<std::string> opts;
    for (const auto& n : loc.connected) opts.push_back("Отправиться: " + world_.locations().at(n).name);
    opts.push_back("Остаться");
    int c = UI::chooseFromMenu("Текущая локация: " + loc.name, opts);
    if (c <= static_cast<int>(loc.connected.size())) {
        currentLocation_ = loc.connected[c - 1];
        UI::printLog("Вы прибыли в " + world_.locations().at(currentLocation_).name);
        randomEvent();
    }
}

void Game::randomEvent() {
    int r = Dice::roll(100);
    if (r < 30) {
        const auto& enemies = world_.locations().at(currentLocation_).enemies;
        if (!enemies.empty()) {
            auto e = world_.enemies().at(enemies[Dice::roll(static_cast<int>(enemies.size())) - 1]);
            if (!combat_.fight(player_, e)) {
                UI::printLog("Поражение. Стража спасла вас и доставила в Светоград.");
                player_.gold = std::max(0, player_.gold - 25);
                player_.stats.hp = player_.stats.maxHp / 2;
                currentLocation_ = "gorod_svetograd";
            }
        }
    } else if (r < 55) {
        UI::printLog("Вы нашли тайник путешественника.");
        player_.gold += 20;
        player_.inventory.addItem(world_.items().at("antidote"));
    } else if (r < 75) {
        UI::printLog("Засада разбойников! Вы уходите, но получаете ранение.");
        player_.stats.hp -= 10;
    } else {
        UI::printHint("Дорога спокойна. Ничего не произошло.");
    }
}

void Game::interactNPC() {
    const auto& npcIds = world_.locations().at(currentLocation_).npcs;
    if (npcIds.empty()) { UI::printLog("Здесь не с кем говорить."); return; }
    std::vector<std::string> opts;
    for (const auto& id : npcIds) opts.push_back(world_.npcs().at(id).name);
    int c = UI::chooseFromMenu("Выберите NPC:", opts);
    auto& npc = world_.npcs().at(npcIds[c - 1]);
    bool success = dialogue_.runConversation(player_, npc);
    if (success && !npc.offeredQuests.empty()) {
        for (const auto& q : npc.offeredQuests) quests_.acceptQuest(q);
        UI::printLog("Получены задания от " + npc.name);
    }
}

void Game::visitShop() {
    UI::printTitle("Торговля и ремонт");
    int c = UI::chooseFromMenu("Выберите действие:", {
        "Купить зелье лечения (15)",
        "Купить огненную бомбу (25)",
        "Купить свиток льда (30)",
        "Продать первый предмет (+10)",
        "Ремонт доспеха (+1 AC за 25)",
        "Уйти"
    });
    if (c == 1 && player_.gold >= 15) {
        player_.gold -= 15; player_.inventory.addItem(world_.items().at("health_potion"));
    } else if (c == 2 && player_.gold >= 25) {
        player_.gold -= 25; player_.inventory.addItem(world_.items().at("fire_bomb"));
    } else if (c == 3 && player_.gold >= 30) {
        player_.gold -= 30; player_.inventory.addItem(world_.items().at("scroll_frost"));
    } else if (c == 4 && !player_.inventory.items().empty()) {
        player_.gold += 10; player_.inventory.removeItem(player_.inventory.items().front().id);
    } else if (c == 5 && player_.gold >= 25) {
        player_.gold -= 25; player_.stats.ac += 1;
    }
}

void Game::useInventoryOutsideCombat() {
    auto& inv = player_.inventory.itemsMutable();
    for (size_t i = 0; i < inv.size(); ++i) {
        if (!inv[i].consumable) continue;
        const auto item = inv[i];
        UI::printLog("Использован предмет: " + item.name);
        if (item.id == "health_potion" || item.id == "bandage") {
            player_.stats.hp = std::min(player_.stats.maxHp, player_.stats.hp + item.power);
        } else if (item.id == "antidote") {
            player_.activeStatuses[StatusEffect::Poison] = 0;
        } else if (item.id == "elixir_guard") {
            player_.stats.ac += 1;
        }
        inv.erase(inv.begin() + static_cast<long>(i));
        return;
    }
    UI::printLog("Нет подходящих расходников.");
}

void Game::rest() { player_.rest(); UI::printLog("Вы отдыхаете у костра и восстанавливаете силы."); }

void Game::gameLoop() {
    campaignFinished_ = false;
    while (!campaignFinished_ && running_) {
        if (player_.stats.hp <= 0) break;
        UI::printDivider();
        int c = UI::chooseFromMenu("Действия:", {
            "Исследование/путешествие", "Диалог", "Бой", "Магазин", "Отдых", "Инвентарь", "Журнал квестов", "Персонаж", "Сохранить", "Завершить игру"
        });

        if (c == 1) explore();
        else if (c == 2) interactNPC();
        else if (c == 3) {
            const auto& enemies = world_.locations().at(currentLocation_).enemies;
            if (!enemies.empty()) {
                auto e = world_.enemies().at(enemies[Dice::roll(static_cast<int>(enemies.size())) - 1]);
                bool win = combat_.fight(player_, e);
                if (!win) { player_.stats.hp = player_.stats.maxHp / 2; currentLocation_ = "gorod_svetograd"; }
                else if (e.id == "boss") { quests_.advanceQuest("q_main_4", player_); campaignFinished_ = true; }
            }
        } else if (c == 4) visitShop();
        else if (c == 5) rest();
        else if (c == 6) useInventoryOutsideCombat();
        else if (c == 7) quests_.showJournal();
        else if (c == 8) showPlayerSheet();
        else if (c == 9) {
            if (saves_.save(player_, currentLocation_, 1)) UI::printLog("Сохранено в слот 1");
        } else {
            running_ = false;
        }

        if (currentLocation_ == "les_serebryany") quests_.advanceQuest("q_main_1", player_);
        if (currentLocation_ == "ruiny_solntsa") quests_.advanceQuest("q_main_2", player_);
        if (currentLocation_ == "hram_rassveta") quests_.advanceQuest("q_main_3", player_);

        if (player_.level >= 4 && quests_.isCompleted("q_main_1")) quests_.acceptQuest("q_main_2");
        if (player_.level >= 7 && quests_.isCompleted("q_main_2")) quests_.acceptQuest("q_main_3");
        if (player_.level >= 10 && quests_.isCompleted("q_main_3")) quests_.acceptQuest("q_main_4");

        if (campaignFinished_) {
            UI::printTitle("Финал");
            if (player_.karma >= 3) UI::printLog("Вы спасли королевство и объединили фракции.");
            else if (player_.reputation[Faction::Bandits] > 3) UI::printLog("Вы возглавили теневой союз разбойников.");
            else UI::printLog("Тиран повержен, но будущее региона неопределенно.");
        }
    }
}

} // namespace rpg

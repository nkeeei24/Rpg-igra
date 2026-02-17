#pragma once

#include "core/Character.h"

namespace rpg {

class QuestSystem {
public:
    void addQuest(const Quest& quest);
    void acceptQuest(const std::string& questId);
    void advanceQuest(const std::string& questId, Character& player);
    void completeQuest(const std::string& questId, Character& player);
    void showJournal() const;
    bool isCompleted(const std::string& questId) const;

private:
    std::map<std::string, Quest> quests_;
    std::vector<std::string> active_;
};

} // namespace rpg

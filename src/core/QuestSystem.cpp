#include "core/QuestSystem.h"

#include <algorithm>
#include <iostream>

namespace rpg {

void QuestSystem::addQuest(const Quest& quest) { quests_[quest.id] = quest; }

void QuestSystem::acceptQuest(const std::string& questId) {
    if (quests_.count(questId) && std::find(active_.begin(), active_.end(), questId) == active_.end()) {
        active_.push_back(questId);
    }
}

void QuestSystem::advanceQuest(const std::string& questId, Character& player) {
    auto it = quests_.find(questId);
    if (it == quests_.end()) return;
    for (auto& stage : it->second.stages) {
        if (!stage.completed) {
            stage.completed = true;
            if (&stage == &it->second.stages.back()) completeQuest(questId, player);
            return;
        }
    }
}

void QuestSystem::completeQuest(const std::string& questId, Character& player) {
    auto it = quests_.find(questId);
    if (it == quests_.end() || it->second.completed) return;
    it->second.completed = true;
    player.karma += it->second.karmaDelta;
    for (const auto& [f, v] : it->second.reputationDelta) player.reputation[f] += v;
    player.gainXp(90);
    player.gold += 50;
}

void QuestSystem::showJournal() const {
    std::cout << "\nЖурнал заданий:\n";
    for (const auto& qid : active_) {
        const auto& q = quests_.at(qid);
        std::cout << "- " << q.title << (q.completed ? " [ЗАВЕРШЕНО]" : "") << "\n";
        for (const auto& s : q.stages) {
            std::cout << "    " << (s.completed ? "[x] " : "[ ] ") << s.description << "\n";
        }
    }
}

bool QuestSystem::isCompleted(const std::string& questId) const {
    auto it = quests_.find(questId);
    return it != quests_.end() && it->second.completed;
}

} // namespace rpg

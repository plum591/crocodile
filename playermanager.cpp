#include "playermanager.h"

void PlayerManager::addPlayer(const std::string& name) {
    m_players.push_back(Player(name));
}

void PlayerManager::removePlayer(int index) {
    // Проверяем, что номер правильный (не вышли за границы списка)
    if (index >= 0 && index < (int)m_players.size()) {
        m_players.erase(m_players.begin() + index);
    }
}

void PlayerManager::renamePlayer(int index, const std::string& newName) {
    if (index >= 0 && index < (int)m_players.size()) {
        m_players[index].setName(newName);
    }
}

int PlayerManager::count() const {
    return (int)m_players.size();
}

Player& PlayerManager::player(int index) {
    return m_players[index];
}

const Player& PlayerManager::player(int index) const {
    return m_players[index];
}

void PlayerManager::addPoints(int index, int points) {
    if (index >= 0 && index < (int)m_players.size()) {
        m_players[index].addPoints(points);
    }
}

void PlayerManager::resetAllScores() {
    for (int i = 0; i < (int)m_players.size(); i++) {
        m_players[i].resetScore();
    }
}

int PlayerManager::findWinner(int targetScore) const {
    for (int i = 0; i < (int)m_players.size(); i++) {
        if (m_players[i].score() >= targetScore) {
            return i;   // нашли победителя
        }
    }
    return -1;          // победителя пока нет
}

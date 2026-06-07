#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <string>
#include <vector>
#include "player.h"

// Менеджер игроков. Хранит список всех игроков, добавляет/удаляет/переименовывает,
// начисляет очки и ищет победителя.
// Соответствует компоненту "Player Manager" из диаграммы компонентов.
class PlayerManager {
public:
    // --- Управление профилями (ТЗ 3.1.2, диаграмма ВИ: "Управлять профилями") ---
    void addPlayer(const std::string& name);                 // добавить игрока
    void removePlayer(int index);                            // удалить игрока по номеру
    void renamePlayer(int index, const std::string& newName);// переименовать игрока

    // --- Доступ к игрокам ---
    int count() const;                       // сколько игроков
    Player& player(int index);               // получить игрока по номеру (для изменения)
    const Player& player(int index) const;   // получить игрока по номеру (только чтение)

    // --- Очки (Scoring) ---
    void addPoints(int index, int points);   // начислить/отнять очки игроку
    void resetAllScores();                   // обнулить очки всем (новая игра)

    // --- Победитель ---
    // Возвращает номер первого игрока, набравшего targetScore и больше.
    // Если такого нет — возвращает -1.
    int findWinner(int targetScore) const;

private:
    std::vector<Player> m_players;   // список всех игроков
};

#endif // PLAYERMANAGER_H

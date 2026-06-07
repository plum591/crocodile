#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include <vector>
#include "difficulty.h"
#include "playermanager.h"
#include "worddictionary.h"

// Главный "мозг" игры. Управляет сессией, ходами, словами, таймером и очками.
// Соответствует компоненту "Game Engine"
// (Session Manager + Logic Controller + Scoring System + Timer).
//
// Важно: таймер здесь НЕ привязан к Qt. Движок просто хранит "сколько секунд осталось".
// В графическом интерфейсе будет QTimer, который раз в секунду вызывает tick().
// Так логика остаётся простой и не зависит от Qt.
class GameEngine {
public:
    GameEngine();

    // ================= СЕССИЯ (Session Manager) =================
    // Начать новую игру: задаём сложность и нужное для победы число очков (по ТЗ — 20).
    void startSession(Difficulty difficulty, int targetScore = 20);
    bool isSessionActive() const;            // идёт ли сейчас игра

    // Доступ к менеджеру игроков (добавить/удалить/переименовать игроков до старта).
    PlayerManager& players();

    // ================= ХОД / РАУНД (Logic Controller) =================
    // Начать новый ход: вычисляем художника и получаем 3 слова на выбор.
    void startTurn();

    int currentRound() const;                // номер текущего раунда
    int currentArtistIndex() const;          // номер игрока-художника
    std::string currentArtistName() const;   // имя художника

    // Три случайных слова, которые показываем художнику (ТЗ 3.1.5).
    std::vector<std::string> wordChoices() const;

    // Художник выбрал слово (по номеру 0..2). ТЗ: "Выбрать слово".
    void chooseWord(int wordIndex);
    std::string chosenWord() const;          // загаданное слово

    // ================= ТАЙМЕР (Timer) =================
    // Запустить таймер хода (ТЗ 3.1.7 — 60 секунд).
    void startTimer(int seconds = 60);
    void tick();                             // вызывать раз в секунду из QTimer
    int secondsLeft() const;                 // сколько секунд осталось
    bool isTimeUp() const;                   // время вышло?
    void endTurnEarly();                     // досрочно завершить ход (ТЗ 3.1.9)

    // ================= ОЧКИ (Scoring System) =================
    // Начислить очки игроку по номеру. По ТЗ 3.1.10/3.1.11 за угадывание даётся +3.
    // points может быть и отрицательным, если нажали "минус" на экране расчёта.
    void addPoints(int playerIndex, int points);

    // ================= КОНЕЦ ИГРЫ =================
    bool isGameOver() const;                 // кто-то набрал нужные очки?
    int winnerIndex() const;                 // номер победителя (или -1)
    std::string winnerName() const;          // имя победителя

    // Полный сброс игры для новой партии (ТЗ: "Сбросить игру", "Новая игра").
    void resetGame();

private:
    PlayerManager  m_players;     // игроки
    WordDictionary m_dictionary;  // словарь слов
    Difficulty     m_difficulty;  // выбранный режим
    int            m_targetScore; // сколько очков нужно для победы (20)

    bool m_sessionActive;         // идёт ли игра
    int  m_round;                 // номер раунда (1, 2, 3, ...)
    int  m_artistIndex;           // кто сейчас рисует

    std::vector<std::string> m_wordChoices; // три слова на выбор
    std::string m_chosenWord;               // выбранное слово

    int  m_secondsLeft;           // осталось секунд на таймере
};

#endif // GAMEENGINE_H

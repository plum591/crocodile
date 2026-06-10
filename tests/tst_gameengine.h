#pragma once

#include <QtTest>
#include "gameengine.h"

// ============================================================
//  Вспомогательная функция: создать игру с N игроками
// ============================================================
static GameEngine makeGame(int playerCount = 3,
                           Difficulty diff = Difficulty::Medium,
                           int target = 20)
{
    GameEngine g;
    const char* names[] = {"Аня", "Боря", "Вика", "Гена", "Дима"};
    for (int i = 0; i < playerCount && i < 5; ++i)
        g.players().addPlayer(names[i]);
    g.startSession(diff, target);
    return g;
}

// ============================================================
//  Тесты GameEngine
// ============================================================
class TestGameEngine : public QObject {
    Q_OBJECT

private slots:

    // ─── startSession ───────────────────────────────────────

    void test_startSession_isActive() {
        GameEngine g = makeGame();
        QVERIFY(g.isSessionActive());
    }

    void test_startSession_resetsScores() {
        GameEngine g;
        g.players().addPlayer("Аня");
        g.players().addPlayer("Боря");
        g.players().addPoints(0, 10);
        g.startSession(Difficulty::Easy, 20);
        QCOMPARE(g.players().player(0).score(), 0);
    }

    // ─── startTurn / currentRound ────────────────────────────

    void test_startTurn_incrementsRound() {
        GameEngine g = makeGame();
        g.startTurn();
        QCOMPARE(g.currentRound(), 1);
        g.startTurn();
        QCOMPARE(g.currentRound(), 2);
    }

    void test_startTurn_artistRotates() {
        // 3 игрока → раунды 1,2,3 → художники 0,1,2
        GameEngine g = makeGame(3);
        g.startTurn(); QCOMPARE(g.currentArtistIndex(), 0);
        g.startTurn(); QCOMPARE(g.currentArtistIndex(), 1);
        g.startTurn(); QCOMPARE(g.currentArtistIndex(), 2);
        g.startTurn(); QCOMPARE(g.currentArtistIndex(), 0); // по кругу
    }

    void test_startTurn_artistNameMatchesIndex() {
        GameEngine g = makeGame(2);
        g.startTurn();
        std::string expected = g.players().player(g.currentArtistIndex()).name();
        QCOMPARE(g.currentArtistName(), expected);
    }

    // ─── wordChoices ─────────────────────────────────────────

    void test_startTurn_wordChoicesCount() {
        GameEngine g = makeGame();
        g.startTurn();
        QCOMPARE((int)g.wordChoices().size(), 3);
    }

    void test_startTurn_wordChoicesNotEmpty() {
        GameEngine g = makeGame();
        g.startTurn();
        for (const auto& w : g.wordChoices())
            QVERIFY(!w.empty());
    }

    void test_startTurn_wordChoicesAreUnique() {
        GameEngine g = makeGame();
        g.startTurn();
        auto words = g.wordChoices();
        QVERIFY(words[0] != words[1]);
        QVERIFY(words[1] != words[2]);
        QVERIFY(words[0] != words[2]);
    }

    // ─── chooseWord ──────────────────────────────────────────

    void test_chooseWord_validIndex_setsWord() {
        GameEngine g = makeGame();
        g.startTurn();
        g.chooseWord(1);
        QCOMPARE(g.chosenWord(), g.wordChoices()[1]);
    }

    void test_chooseWord_invalidIndex_wordUnchanged() {
        GameEngine g = makeGame();
        g.startTurn();
        g.chooseWord(0);
        std::string before = g.chosenWord();
        g.chooseWord(99);           // невалидный индекс
        QCOMPARE(g.chosenWord(), before);
    }

    void test_chooseWord_afterNewTurn_resets() {
        GameEngine g = makeGame();
        g.startTurn();
        g.chooseWord(0);
        QVERIFY(!g.chosenWord().empty());
        g.startTurn();              // новый ход — слово должно сброситься
        QVERIFY(g.chosenWord().empty());
    }

    // ─── Таймер ──────────────────────────────────────────────

    void test_timer_startsWithGivenSeconds() {
        GameEngine g = makeGame();
        g.startTimer(60);
        QCOMPARE(g.secondsLeft(), 60);
    }

    void test_timer_tickDecrementsSeconds() {
        GameEngine g = makeGame();
        g.startTimer(60);
        g.tick();
        QCOMPARE(g.secondsLeft(), 59);
    }

    void test_timer_doesNotGoBelowZero() {
        GameEngine g = makeGame();
        g.startTimer(1);
        g.tick();
        g.tick(); // лишний tick
        QCOMPARE(g.secondsLeft(), 0);
    }

    void test_timer_isTimeUp_whenZero() {
        GameEngine g = makeGame();
        g.startTimer(1);
        QVERIFY(!g.isTimeUp());
        g.tick();
        QVERIFY(g.isTimeUp());
    }

    void test_timer_endTurnEarly_setsZero() {
        GameEngine g = makeGame();
        g.startTimer(60);
        g.endTurnEarly();
        QVERIFY(g.isTimeUp());
        QCOMPARE(g.secondsLeft(), 0);
    }

    // ─── addPoints / isGameOver ──────────────────────────────

    void test_addPoints_updatesScore() {
        GameEngine g = makeGame(2);
        g.addPoints(0, 3);
        QCOMPARE(g.players().player(0).score(), 3);
    }

    void test_isGameOver_falseWhenNooneReachedTarget() {
        GameEngine g = makeGame(2);
        g.addPoints(0, 15);
        QVERIFY(!g.isGameOver());
    }

    void test_isGameOver_trueWhenTargetReached() {
        GameEngine g = makeGame(2, Difficulty::Easy, 20);
        g.addPoints(0, 20);
        QVERIFY(g.isGameOver());
    }

    void test_winnerIndex_correctPlayer() {
        GameEngine g = makeGame(3, Difficulty::Easy, 20);
        g.addPoints(2, 21);
        QCOMPARE(g.winnerIndex(), 2);
    }

    void test_winnerName_matchesPlayer() {
        GameEngine g = makeGame(2, Difficulty::Easy, 20);
        g.addPoints(1, 20);
        QCOMPARE(g.winnerName(), g.players().player(1).name());
    }

    void test_winnerIndex_noWinner_returnsMinusOne() {
        GameEngine g = makeGame(2);
        QCOMPARE(g.winnerIndex(), -1);
    }

    // ─── resetGame ───────────────────────────────────────────

    void test_resetGame_sessionNotActive() {
        GameEngine g = makeGame(2);
        g.resetGame();
        QVERIFY(!g.isSessionActive());
    }

    void test_resetGame_noPlayers() {
        GameEngine g = makeGame(3);
        g.resetGame();
        QCOMPARE(g.players().count(), 0);
    }

    void test_resetGame_roundIsZero() {
        GameEngine g = makeGame(2);
        g.startTurn();
        g.resetGame();
        QCOMPARE(g.currentRound(), 0);
    }
};
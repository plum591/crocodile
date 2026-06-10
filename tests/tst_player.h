#pragma once
#include <QtTest>
#include "player.h"
#include "playermanager.h"
//  Тесты класса Player
class TestPlayer : public QObject {
    Q_OBJECT
private slots:
    void test_constructor_setsName() {
        Player p("Аня");
        QCOMPARE(p.name(), std::string("Аня"));
    }
    void test_constructor_scoreIsZero() {
        Player p("Боря");
        QCOMPARE(p.score(), 0);
    }
    // addPoints
    void test_addPoints_positive() {
        Player p("Вика");
        p.addPoints(3);
        QCOMPARE(p.score(), 3);
    }
    void test_addPoints_negative_doesNotGoBelowZero() {
        Player p("Гена");
        p.addPoints(-5);          // нельзя уйти в минус
        QCOMPARE(p.score(), 0);
    }
    void test_addPoints_negativeFromPositive() {
        Player p("Дима");
        p.addPoints(6);
        p.addPoints(-3);
        QCOMPARE(p.score(), 3);
    }
    void test_addPoints_negativeExceedsScore_clampToZero() {
        Player p("Ева");
        p.addPoints(3);
        p.addPoints(-10);         // 3 - 10 < 0  → должно стать 0
        QCOMPARE(p.score(), 0);
    }
    void test_addPoints_accumulates() {
        Player p("Женя");
        p.addPoints(3);
        p.addPoints(3);
        p.addPoints(3);
        QCOMPARE(p.score(), 9);
    }
    // resetScore
    void test_resetScore_setsZero() {
        Player p("Зоя");
        p.addPoints(15);
        p.resetScore();
        QCOMPARE(p.score(), 0);
    }
    // setName
    void test_setName_changesName() {
        Player p("Иван");
        p.setName("Игорь");
        QCOMPARE(p.name(), std::string("Игорь"));
    }
};
//  Тесты класса PlayerManager
class TestPlayerManager : public QObject {
    Q_OBJECT
private slots:
    // addPlayer / count
    void test_addPlayer_incrementsCount() {
        PlayerManager pm;
        QCOMPARE(pm.count(), 0);
        pm.addPlayer("Аня");
        QCOMPARE(pm.count(), 1);
        pm.addPlayer("Боря");
        QCOMPARE(pm.count(), 2);
    }
    void test_addPlayer_nameIsCorrect() {
        PlayerManager pm;
        pm.addPlayer("Аня");
        QCOMPARE(pm.player(0).name(), std::string("Аня"));
    }
    // removePlayer
    void test_removePlayer_decrementsCount() {
        PlayerManager pm;
        pm.addPlayer("Аня");
        pm.addPlayer("Боря");
        pm.removePlayer(0);
        QCOMPARE(pm.count(), 1);
    }
    void test_removePlayer_correctPlayerRemains() {
        PlayerManager pm;
        pm.addPlayer("Аня");
        pm.addPlayer("Боря");
        pm.removePlayer(0);                          // удаляем Аню
        QCOMPARE(pm.player(0).name(), std::string("Боря"));
    }
    void test_removePlayer_invalidIndex_noChange() {
        PlayerManager pm;
        pm.addPlayer("Аня");
        pm.removePlayer(99);                         // несуществующий индекс
        QCOMPARE(pm.count(), 1);
    }

    // addPoints
    void test_addPoints_correctPlayer() {
        PlayerManager pm;
        pm.addPlayer("Аня");
        pm.addPlayer("Боря");
        pm.addPoints(1, 6);
        QCOMPARE(pm.player(0).score(), 0);
        QCOMPARE(pm.player(1).score(), 6);
    }
    // resetAllScores
    void test_resetAllScores_allBecomZero() {
        PlayerManager pm;
        pm.addPlayer("Аня");
        pm.addPlayer("Боря");
        pm.addPoints(0, 9);
        pm.addPoints(1, 12);
        pm.resetAllScores();
        QCOMPARE(pm.player(0).score(), 0);
        QCOMPARE(pm.player(1).score(), 0);
    }
    // findWinner
    void test_findWinner_noWinner_returnsMinusOne() {
        PlayerManager pm;
        pm.addPlayer("Аня");
        pm.addPlayer("Боря");
        pm.addPoints(0, 15);
        QCOMPARE(pm.findWinner(20), -1);
    }
    void test_findWinner_exactTarget_returnsIndex() {
        PlayerManager pm;
        pm.addPlayer("Аня");
        pm.addPlayer("Боря");
        pm.addPoints(1, 20);
        QCOMPARE(pm.findWinner(20), 1);
    }
    void test_findWinner_exceedsTarget_returnsIndex() {
        PlayerManager pm;
        pm.addPlayer("Аня");
        pm.addPoints(0, 23);
        QCOMPARE(pm.findWinner(20), 0);
    }
    void test_findWinner_firstWinnerTakesPriority() {
        // Если оба набрали нужное — возвращается первый по индексу
        PlayerManager pm;
        pm.addPlayer("Аня");
        pm.addPlayer("Боря");
        pm.addPoints(0, 20);
        pm.addPoints(1, 20);
        QCOMPARE(pm.findWinner(20), 0);
    }
};
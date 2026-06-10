#pragma once
#include <QtTest>
#include <algorithm>
#include "worddictionary.h"
class TestWordDictionary : public QObject {
    Q_OBJECT
private slots:
    // Количество возвращаемых слов
    void test_getRandomWords_easy_returnsThree() {
        WordDictionary d;
        auto words = d.getRandomWords(Difficulty::Easy, 3);
        QCOMPARE((int)words.size(), 3);
    }
    void test_getRandomWords_medium_returnsThree() {
        WordDictionary d;
        auto words = d.getRandomWords(Difficulty::Medium, 3);
        QCOMPARE((int)words.size(), 3);
    }
    void test_getRandomWords_hard_returnsThree() {
        WordDictionary d;
        auto words = d.getRandomWords(Difficulty::Hard, 3);
        QCOMPARE((int)words.size(), 3);
    }
    // Слова не пустые
    void test_getRandomWords_easy_allNonEmpty() {
        WordDictionary d;
        for (const auto& w : d.getRandomWords(Difficulty::Easy, 3))
            QVERIFY(!w.empty());
    }
    void test_getRandomWords_medium_allNonEmpty() {
        WordDictionary d;
        for (const auto& w : d.getRandomWords(Difficulty::Medium, 3))
            QVERIFY(!w.empty());
    }
    void test_getRandomWords_hard_allNonEmpty() {
        WordDictionary d;
        for (const auto& w : d.getRandomWords(Difficulty::Hard, 3))
            QVERIFY(!w.empty());
    }
    // Слова уникальны внутри одного запроса
    void test_getRandomWords_easy_uniqueWithinCall() {
        WordDictionary d;
        auto words = d.getRandomWords(Difficulty::Easy, 3);
        QVERIFY(words[0] != words[1]);
        QVERIFY(words[1] != words[2]);
        QVERIFY(words[0] != words[2]);
    }
    void test_getRandomWords_medium_uniqueWithinCall() {
        WordDictionary d;
        auto words = d.getRandomWords(Difficulty::Medium, 3);
        QVERIFY(words[0] != words[1]);
        QVERIFY(words[1] != words[2]);
        QVERIFY(words[0] != words[2]);
    }
    void test_getRandomWords_hard_uniqueWithinCall() {
        WordDictionary d;
        auto words = d.getRandomWords(Difficulty::Hard, 3);
        QVERIFY(words[0] != words[1]);
        QVERIFY(words[1] != words[2]);
        QVERIFY(words[0] != words[2]);
    }
    // Слова принадлежат правильному словарю
    void test_getRandomWords_easy_wordsFromEasyList() {
        // Известные слова из списка (из worddictionary.cpp)
        const std::vector<std::string> easyKnown = {
            "дом","кот","солнце","рыба","ключ","ворон","гриб",
            "мяч","цветок","звезда","яблоко","лист","дерево","рука","глаз"
        };
        WordDictionary d;
        auto words = d.getRandomWords(Difficulty::Easy, 3);
        for (const auto& w : words) {
            bool found = std::find(easyKnown.begin(), easyKnown.end(), w) != easyKnown.end();
            QVERIFY2(found, ("Слово не из Easy-словаря: " + w).c_str());
        }
    }
    void test_getRandomWords_hard_wordsFromHardList() {
        const std::vector<std::string> hardKnown = {
            "эволюция","гравитация","философия","демократия","бесконечность",
            "ностальгия","симметрия","абстракция","парадокс","гипотеза",
            "перспектива","инфляция","дипломатия","энтропия","свобода"
        };
        WordDictionary d;
        auto words = d.getRandomWords(Difficulty::Hard, 3);
        for (const auto& w : words) {
            bool found = std::find(hardKnown.begin(), hardKnown.end(), w) != hardKnown.end();
            QVERIFY2(found, ("Слово не из Hard-словаря: " + w).c_str());
        }
    }
    // Разные сложности не пересекаются
    void test_easyAndHard_wordsDontOverlap() {
        // Запрашиваем по 3 слова много раз и проверяем, что easy-слово
        // никогда не попадёт в hard (списки заведомо разные)
        const std::vector<std::string> easyKnown = {
            "дом","кот","солнце","рыба","ключ","ворон","гриб",
            "мяч","цветок","звезда","яблоко","лист","дерево","рука","глаз"
        };
        WordDictionary d;
        auto hardWords = d.getRandomWords(Difficulty::Hard, 3);
        for (const auto& w : hardWords) {
            bool inEasy = std::find(easyKnown.begin(), easyKnown.end(), w) != easyKnown.end();
            QVERIFY(!inEasy);
        }
    }
    // Запрос count=1
    void test_getRandomWords_countOne_returnsOne() {
        WordDictionary d;
        auto words = d.getRandomWords(Difficulty::Medium, 1);
        QCOMPARE((int)words.size(), 1);
    }
    // Случайность
    void test_getRandomWords_randomness_notAlwaysSame() {
        // За 20 вызовов наборы не должны быть полностью идентичными хотя бы раз
        WordDictionary d;
        auto first = d.getRandomWords(Difficulty::Medium, 3);
        bool differs = false;
        for (int i = 0; i < 20; ++i) {
            auto next = d.getRandomWords(Difficulty::Medium, 3);
            if (next != first) { differs = true; break; }
        }
        QVERIFY(differs);
    }
};
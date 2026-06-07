#include "worddictionary.h"
#include <algorithm>   // для std::shuffle
#include <random>      // для генератора случайных чисел

WordDictionary::WordDictionary() {
    // Лёгкие слова — простые предметы, которые легко нарисовать.
    m_easyWords = {
        "дом", "кот", "солнце", "рыба", "ключ", "ворон", "гриб",
        "мяч", "цветок", "звезда", "яблоко", "лист", "дерево", "рука", "глаз"
    };

    // Средние слова — посложнее, требуют деталей.
    m_mediumWords = {
        "велосипед", "телефон", "гитара", "замок", "ракета", "чемодан",
        "фонарь", "парус", "мельница", "маяк", "скрипка", "телескоп",
        "аквариум", "компас", "парашют"
    };

    // Сложные слова — абстрактные понятия, нарисовать очень трудно.
    m_hardWords = {
        "эволюция", "гравитация", "философия", "демократия", "бесконечность",
        "ностальгия", "симметрия", "абстракция", "парадокс", "гипотеза",
        "перспектива", "инфляция", "дипломатия", "энтропия", "свобода"
    };
}

const std::vector<std::string>& WordDictionary::listFor(Difficulty difficulty) const {
    if (difficulty == Difficulty::Easy) {
        return m_easyWords;
    } else if (difficulty == Difficulty::Medium) {
        return m_mediumWords;
    } else {
        return m_hardWords;
    }
}

std::vector<std::string> WordDictionary::getRandomWords(Difficulty difficulty, int count) {
    // Берём копию нужного списка, чтобы перемешать её и не испортить оригинал.
    std::vector<std::string> pool = listFor(difficulty);

    // Перемешиваем список в случайном порядке.
    // static — чтобы генератор создавался один раз, а не при каждом вызове.
    static std::mt19937 generator(std::random_device{}());
    std::shuffle(pool.begin(), pool.end(), generator);

    // Берём первые count слов (но не больше, чем есть в списке).
    std::vector<std::string> result;
    for (int i = 0; i < count && i < (int)pool.size(); i++) {
        result.push_back(pool[i]);
    }
    return result;
}

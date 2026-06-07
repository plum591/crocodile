#ifndef DIFFICULTY_H
#define DIFFICULTY_H

// Режим (сложность) игры. От него зависит сложность загаданных слов.
// ТЗ п. 3.1.3: Лёгкий, Средний или Сложный.
enum class Difficulty {
    Easy,    // Лёгкий
    Medium,  // Средний
    Hard     // Сложный
};

#endif // DIFFICULTY_H

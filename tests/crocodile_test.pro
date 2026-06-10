QT += testlib widgets
CONFIG += qt warn_on depend_includepath testcase c++17
TEMPLATE = app
TARGET = crocodile_test

# Файлы тестов лежат здесь же (в папке tests),
# а файлы исходного кода — на уровень выше (../)
SOURCES += \
    main_test.cpp \
    ../player.cpp \
    ../playermanager.cpp \
    ../worddictionary.cpp \
    ../gameengine.cpp \
    ../drawingcanvas.cpp

HEADERS += \
    tst_player.h \
    tst_gameengine.h \
    tst_worddictionary.h \
    tst_drawingcanvas.h \
    ../player.h \
    ../playermanager.h \
    ../worddictionary.h \
    ../gameengine.h \
    ../drawingcanvas.h \
    ../difficulty.h

# Добавляем родительскую папку в пути поиска инклудов,
# чтобы внутри tst_*.h файлов можно было писать #include "player.h" без ../
INCLUDEPATH += ..
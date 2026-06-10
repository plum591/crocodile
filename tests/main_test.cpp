#include <QtTest>
#include <QApplication>

#include "tst_player.h"
#include "tst_gameengine.h"
#include "tst_worddictionary.h"
#include "tst_drawingcanvas.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    int status = 0;
    auto run = [&](QObject* obj) {
        status |= QTest::qExec(obj, argc, argv);
        delete obj;
    };

    run(new TestPlayer);
    run(new TestPlayerManager);
    run(new TestGameEngine);
    run(new TestWordDictionary);
    run(new TestDrawingCanvas);

    return status;
}
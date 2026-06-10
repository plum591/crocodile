#pragma once

#include <QtTest>
#include <QApplication>
#include <QColor>
#include "drawingcanvas.h"

class TestDrawingCanvas : public QObject {
    Q_OBJECT

private slots:

    // ─── Инициализация холста ──────────────────────────────────

    void test_initialBackground_isWhite() {
        DrawingCanvas c;
        c.resize(200, 200);
        c.show();
        QVERIFY(QTest::qWaitForWindowExposed(&c));

        QImage img = c.grab().toImage();
        QColor topLeft = img.pixelColor(5, 5);
        QCOMPARE(topLeft, QColor(Qt::white));
    }

    // ─── Очистка холста ────────────────────────────────────────

    void test_clear_resetsToWhite() {
        DrawingCanvas c;
        c.resize(200, 200);
        c.show();
        QVERIFY(QTest::qWaitForWindowExposed(&c));

        c.setPenColor(Qt::red);
        c.setPenWidth(10);

        // Используем стандартный QTest для симуляции мазка
        QTest::mousePress(&c, Qt::LeftButton, Qt::NoModifier, QPoint(50, 50));
        QTest::mouseMove(&c, QPoint(150, 150));
        QTest::mouseRelease(&c, Qt::LeftButton, Qt::NoModifier, QPoint(150, 150));

        c.clear();
        QCoreApplication::processEvents();

        QImage img = c.grab().toImage();
        QColor center = img.pixelColor(100, 100);
        QCOMPARE(center, QColor(Qt::white));
    }

    // ─── Проверка выбора цвета ─────────────────────────────────

    void test_setPenColor_drawsWithChosenColor() {
        DrawingCanvas c;
        c.resize(200, 200);
        c.show();
        QVERIFY(QTest::qWaitForWindowExposed(&c));

        c.setPenColor(Qt::blue);
        c.setPenWidth(24); // Делаем линию потолще, чтобы точно попасть тестом

        // Рисуем явную линию из (80, 80) в (120, 120)
        QTest::mousePress(&c, Qt::LeftButton, Qt::NoModifier, QPoint(80, 80));
        QTest::mouseMove(&c, QPoint(120, 120));
        QTest::mouseRelease(&c, Qt::LeftButton, Qt::NoModifier, QPoint(120, 120));

        // Даем Qt время отрендерить графику в буфер
        QCoreApplication::processEvents();
        QTest::qWait(50);

        QImage img = c.grab().toImage();
        QColor drawn = img.pixelColor(100, 100); // Проверяем строго центр мазка

        QVERIFY2(drawn != QColor(Qt::white), "Пиксель остался белым! Рисование не произошло.");
    }

    // ─── Проверка ластика ──────────────────────────────────────

    void test_setEraser_erasesDrawnPixel() {
        DrawingCanvas c;
        c.resize(200, 200);
        c.show();
        QVERIFY(QTest::qWaitForWindowExposed(&c));

        // 1. Сначала рисуем чёрную жирную линию
        c.setPenColor(Qt::black);
        c.setPenWidth(24);
        QTest::mousePress(&c, Qt::LeftButton, Qt::NoModifier, QPoint(80, 80));
        QTest::mouseMove(&c, QPoint(120, 120));
        QTest::mouseRelease(&c, Qt::LeftButton, Qt::NoModifier, QPoint(120, 120));
        QCoreApplication::processEvents();

        // Проверяем, что след остался
        {
            QImage img = c.grab().toImage();
            QVERIFY(img.pixelColor(100, 100) != QColor(Qt::white));
        }

        // 2. Включаем ластик и проводим по той же траектории
        c.setEraser(true);
        QTest::mousePress(&c, Qt::LeftButton, Qt::NoModifier, QPoint(80, 80));
        QTest::mouseMove(&c, QPoint(120, 120));
        QTest::mouseRelease(&c, Qt::LeftButton, Qt::NoModifier, QPoint(120, 120));
        QCoreApplication::processEvents();
        QTest::qWait(50);

        QImage img = c.grab().toImage();
        QCOMPARE(img.pixelColor(100, 100), QColor(Qt::white));
    }

    // ─── Автоматический сброс ластика при смене цвета ─────────

    void test_setPenColor_disablesEraser() {
        DrawingCanvas c;
        c.resize(200, 200);
        c.show();
        QVERIFY(QTest::qWaitForWindowExposed(&c));

        c.setEraser(true);
        c.setPenColor(Qt::red); // Этот вызов должен внутри сделать setEraser(false)

        c.setPenWidth(24);
        QTest::mousePress(&c, Qt::LeftButton, Qt::NoModifier, QPoint(80, 80));
        QTest::mouseMove(&c, QPoint(120, 120));
        QTest::mouseRelease(&c, Qt::LeftButton, Qt::NoModifier, QPoint(120, 120));
        QCoreApplication::processEvents();
        QTest::qWait(50);

        QImage img = c.grab().toImage();
        // Если ластик выключился, тут будет красный цвет (не белый)
        QVERIFY(img.pixelColor(100, 100) != QColor(Qt::white));
    }

    // ─── Проверка толщины линии ────────────────────────────────

    void test_setPenWidth_thinLine_lessCoverage() {
        auto countNonWhite = [](DrawingCanvas& c) -> int {
            QImage img = c.grab().toImage();
            int cnt = 0;
            for (int y = 50; y <= 150; ++y)
                for (int x = 50; x <= 150; ++x)
                    if (img.pixelColor(x, y) != QColor(Qt::white)) ++cnt;
            return cnt;
        };

        DrawingCanvas cThin, cThick;
        for (DrawingCanvas* cv : {&cThin, &cThick}) {
            cv->resize(200, 200);
            cv->show();
            QVERIFY(QTest::qWaitForWindowExposed(cv));
        }

        cThin.setPenColor(Qt::black);  cThin.setPenWidth(2);
        cThick.setPenColor(Qt::black); cThick.setPenWidth(20);

        // Тонкий мазок
        QTest::mousePress(&cThin, Qt::LeftButton, Qt::NoModifier, QPoint(80, 80));
        QTest::mouseMove(&cThin, QPoint(120, 120));
        QTest::mouseRelease(&cThin, Qt::LeftButton, Qt::NoModifier, QPoint(120, 120));

        // Толстый мазок
        QTest::mousePress(&cThick, Qt::LeftButton, Qt::NoModifier, QPoint(80, 80));
        QTest::mouseMove(&cThick, QPoint(120, 120));
        QTest::mouseRelease(&cThick, Qt::LeftButton, Qt::NoModifier, QPoint(120, 120));

        QCoreApplication::processEvents();
        QTest::qWait(50);

        // Тонкая линия должна закрасить значительно меньше пикселей, чем жирная
        QVERIFY(countNonWhite(cThin) < countNonWhite(cThick));
    }

    // ─── Защита от рисования без нажатия ───────────────────────

    void test_noDrawWithoutMousePress() {
        DrawingCanvas c;
        c.resize(200, 200);
        c.show();
        QVERIFY(QTest::qWaitForWindowExposed(&c));

        c.setPenColor(Qt::black);
        c.setPenWidth(10);

        // Просто двигаем мышь БЕЗ нажатия (через QTest::mouseMove)
        QTest::mouseMove(&c, QPoint(100, 100));
        QCoreApplication::processEvents();

        QImage img = c.grab().toImage();
        QCOMPARE(img.pixelColor(100, 100), QColor(Qt::white));
    }
};
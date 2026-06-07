QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    drawingcanvas.cpp \
    gameengine.cpp \
    main.cpp \
    mainwindow.cpp \
    player.cpp \
    playermanager.cpp \
    worddictionary.cpp

HEADERS += \
    difficulty.h \
    drawingcanvas.h \
    gameengine.h \
    mainwindow.h \
    player.h \
    playermanager.h \
    worddictionary.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boardposition.cpp \
    chessboard.cpp \
    chessboardqgraphicsitem.cpp \
    chessgame.cpp \
    chesspiece.cpp \
    chesspieceqgraphicsitem.cpp \
    chesswidget.cpp \
    main.cpp \
    chesswindow.cpp

HEADERS += \
    boardposition.h \
    chessboard.h \
    chessboardqgraphicsitem.h \
    chessgame.h \
    chesspiece.h \
    chesspieceqgraphicsitem.h \
    chesswidget.h \
    chesswindow.h \
    constants.h \
    gamestate.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

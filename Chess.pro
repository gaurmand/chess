QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chess/board.cpp \
    chess/boardpos.cpp \
    chess/game.cpp \
    chess/move.cpp \
    chess/piece.cpp \
    chess/position.cpp \
    chess/state.cpp \
    tests/board_test.cpp \
    tests/boardpos_test.cpp \
    tests/game_compute_move_test.cpp \
    tests/game_perform_move_test.cpp \
    tests/game_result_test.cpp \
    tests/move_test.cpp \
    tests/piece_test.cpp \
    tests/position_test.cpp \
    tests/state_test.cpp \
    ui/chessboarditem.cpp \
    ui/chessboardscene.cpp \
    ui/chesspieceitem.cpp \
    ui/chesswidget.cpp \
    ui/chesswindow.cpp \
    ui/main.cpp \
    ui/ui.cpp

HEADERS += \
    chess/board.h \
    chess/boardpos.h \
    chess/common.h \
    chess/game.h \
    chess/move.h \
    chess/piece.h \
    chess/position.h \
    chess/state.h \
    tests/test.h \
    ui/chessboarditem.h \
    ui/chessboardscene.h \
    ui/chesspieceitem.h \
    ui/chesswidget.h \
    ui/chesswindow.h \
    ui/ui.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

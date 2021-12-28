#ifndef CHESSWINDOW_H
#define CHESSWINDOW_H

#include <QMainWindow>

#include "chess/game.h"

class ChessWidget;

class ChessWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChessWindow(QWidget* parent = nullptr);
    ~ChessWindow();

private:
    Chess::Game* game_;
};
#endif // CHESSWINDOW_H

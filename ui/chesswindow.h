#ifndef CHESSWINDOW_H
#define CHESSWINDOW_H

#include <QMainWindow>

#include "playablechessgame.h"
#include "chesswidget.h"
#include "chessboardscene.h"

class ChessWidget;

class ChessWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChessWindow(QWidget* parent = nullptr);
    ~ChessWindow();

private:
    PlayableChessGame game_;
    ChessWidget view_;
    ChessBoardScene* scene_ = nullptr;
};
#endif // CHESSWINDOW_H

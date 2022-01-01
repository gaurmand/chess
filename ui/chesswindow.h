#ifndef CHESSWINDOW_H
#define CHESSWINDOW_H

#include <QMainWindow>
#include <QShortcut>

#include "chesswidget.h"
#include "chessboardscene.h"
#include "chessengine.h"

class ChessWidget;

class ChessWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChessWindow(QWidget* parent = nullptr);
    ~ChessWindow();

private:
    ChessEngine engine_;
    ChessWidget view_;
    InteractiveChessBoardScene scene_;
    QShortcut* refresh_;
};
#endif // CHESSWINDOW_H

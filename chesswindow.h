#ifndef CHESSWINDOW_H
#define CHESSWINDOW_H

#include <QMainWindow>

class ChessWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChessWindow(QWidget *parent = nullptr);
    ~ChessWindow();
};
#endif // CHESSWINDOW_H

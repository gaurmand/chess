#ifndef CHESSWINDOW_H
#define CHESSWINDOW_H

#include <QMainWindow>

class ChessWidget;

class ChessWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChessWindow(QWidget* parent = nullptr);
    ~ChessWindow();

private:
    ChessWidget* chessWidget_;
};
#endif // CHESSWINDOW_H

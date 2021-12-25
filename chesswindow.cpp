#include "chesswindow.h"
#include "chesswidget.h"

ChessWindow::ChessWindow(QWidget *parent)
    : QMainWindow(parent)
{
    chessWidget_ = new ChessWidget(this);
    setCentralWidget(chessWidget_);
    setWindowTitle(tr("Cheese"));
}

ChessWindow::~ChessWindow()
{
}


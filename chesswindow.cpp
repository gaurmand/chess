#include "chesswindow.h"
#include "chesswidget.h"

ChessWindow::ChessWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setCentralWidget(chessWidget);
    setWindowTitle(tr("Cheese"));
}

ChessWindow::~ChessWindow()
{
}


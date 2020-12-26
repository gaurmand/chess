#include "chesswindow.h"
#include "chesswidget.h"

ChessWindow::ChessWindow(QWidget *parent)
    : QMainWindow(parent)
{
    chessWidget = new ChessWidget;
    setCentralWidget(chessWidget);
    setWindowTitle(tr("Cheese"));
}

ChessWindow::~ChessWindow()
{
}


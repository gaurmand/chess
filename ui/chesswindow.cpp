#include "chesswindow.h"
#include "chesswidget.h"
#include "chessboardscene.h"

ChessWindow::ChessWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto game_ = new Chess::Game();
    auto view = new ChessWidget(*game_, this);
    auto scene = new ChessBoardScene(*game_, this);

    view->setScene(scene);
    setCentralWidget(view);
    setWindowTitle(tr("Cheese"));

    connect(view, &ChessWidget::movePerformed, scene, &ChessBoardScene::onPerformMove);
    connect(view, &ChessWidget::readyToReceiveUserMove, scene, &ChessBoardScene::onReadyForNextMove);
    connect(scene, &ChessBoardScene::moveSelected, view, &ChessWidget::completeTurn);

    view->newGame();
}

ChessWindow::~ChessWindow()
{
//    delete game_;
}


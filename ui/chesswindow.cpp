#include "chesswindow.h"
#include "chesswidget.h"
#include "chessboardscene.h"

ChessWindow::ChessWindow(QWidget *parent)
    : QMainWindow(parent),
      engine_(Chess::PlayerType::Human, Chess::PlayerType::AI),
      view_(this)
{

    view_.setScene(&scene_);
    setCentralWidget(&view_);
    setWindowTitle(tr("Cheese"));

    connect(&scene_, &InteractiveChessBoardScene::gameCompleted, &view_, &ChessWidget::showGameCompleteDialog);
    connect(&view_, &ChessWidget::requestNewGame, &scene_, &InteractiveChessBoardScene::newGame);

    connect(&scene_, &InteractiveChessBoardScene::movePerformed, &engine_, &ChessEngine::performMove);
    connect(&scene_, &InteractiveChessBoardScene::readyForEngineMove, &engine_, &ChessEngine::selectMove);
    connect(&scene_, &InteractiveChessBoardScene::gameStarted, &engine_, &ChessEngine::reset);
    connect(&engine_, &ChessEngine::moveSelected, &scene_, &InteractiveChessBoardScene::performMove);

    scene_.newGame();
}

ChessWindow::~ChessWindow()
{
}


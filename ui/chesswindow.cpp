#include "chesswindow.h"
#include "chesswidget.h"
#include "chessboardscene.h"

ChessWindow::ChessWindow(QWidget *parent)
    : QMainWindow(parent),
      game_(Chess::PlayerType::Human, Chess::PlayerType::Human),
      view_(this)
{
    std::vector<std::vector<ChessPieceItem*>> pieceItems;
    game_.initializeChessPieceItems(pieceItems);
    scene_ = new ChessBoardScene(pieceItems, this);
    view_.setScene(scene_);
    setCentralWidget(&view_);
    setWindowTitle(tr("Cheese"));

    connect(&game_, &PlayableChessGame::movePerformed, scene_, &ChessBoardScene::onMovePerformed);
    connect(&game_, &PlayableChessGame::moveFailed, scene_, &ChessBoardScene::onMoveFailed);
    connect(&game_, &PlayableChessGame::waitingForMove, scene_, &ChessBoardScene::enablePlayerMoveSelection);
    connect(&game_, &PlayableChessGame::gameStarted, scene_, &ChessBoardScene::reset);
    connect(&game_, &PlayableChessGame::gameCompleted, scene_, &ChessBoardScene::disablePlayerMoveSelection);
    connect(&game_, &PlayableChessGame::updatedBoardStates, scene_, &ChessBoardScene::updateBoard);

    connect(scene_, &ChessBoardScene::moveSelected, &game_, &PlayableChessGame::performMove);
    connect(scene_, &ChessBoardScene::pieceSelected, &game_, &PlayableChessGame::emitSelectedBPStates);
    connect(scene_, &ChessBoardScene::deselected, &game_, &PlayableChessGame::emitDefaultBPStates);

    connect(&game_, &PlayableChessGame::gameCompleted, &view_, &ChessWidget::showGameCompleteDialog);
    connect(&view_, &ChessWidget::requestNewGame, &game_, &PlayableChessGame::newGame);

    game_.newGame();
}

ChessWindow::~ChessWindow()
{
    scene_->deleteLater();
}


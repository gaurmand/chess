#include "chessboardscene.h"
#include "chesswidget.h"
#include "chesspieceitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>

ChessBoardScene::ChessBoardScene(const Chess::Game& game, QObject* parent):
    QGraphicsScene(parent), game_(game), board_(new ChessBoardItem(game))
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
    addItem(board_);
}

void ChessBoardScene::enableInteraction()
{
    board_->setPiecesMovable(game_.activePlayer());
}

void ChessBoardScene::disableInteraction()
{
    board_->setPiecesMovable(false);
    deselectPiece();
    board_->updatePieces();
}

Chess::BP ChessBoardScene::toChessBoardPosition(QPointF point)
{
    int x = point.x();
    int y = point.y();

    int i = (y / SQUARE_WIDTH);
    int j = (x / SQUARE_WIDTH);
    return Chess::BP(i, j);
}

void ChessBoardScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    onClick(event);
    event->accept();
}

void ChessBoardScene::onClick(QGraphicsSceneMouseEvent* event)
{
    const QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
    if (item == board_)
    {
        onChessBoardClick(event);
    }
    else if(auto pieceItem = qgraphicsitem_cast<const ChessPieceItem*>(item))
    {
        onChessPieceClick(pieceItem);
    }
}

void ChessBoardScene::onChessBoardClick(QGraphicsSceneMouseEvent* event)
{
    if (isSelected_)
    {
        const auto clickedPos = toChessBoardPosition(event->scenePos());
        const auto selectedPiecePos = selectedPiece_->pos();

        if (!attemptMove(selectedPiecePos, clickedPos))
        {
            deselectPiece();
        }
    }
}

void ChessBoardScene::onChessPieceClick(const ChessPieceItem* pieceItem)
{
    const Chess::Player activePlayer = game_.activePlayer();
    const Chess::Piece* clickedPiece = pieceItem->chessPiece();
    const Chess::Player clickedPlayer = clickedPiece->owner();

    if (isSelected_)
    {
        if (clickedPiece == selectedPiece_)
        {
            deselectPiece();
        }
        else if (activePlayer == clickedPlayer)
        {
            selectPiece(clickedPiece);
        }
        else
        {
            const auto clickedPos = clickedPiece->pos();
            const auto selectedPiecePos = selectedPiece_->pos();

            if (!attemptMove(selectedPiecePos, clickedPos))
            {
                deselectPiece();
            }
        }
    }
    else if(activePlayer == clickedPlayer)
    {
        selectPiece(clickedPiece);
    }
}

void ChessBoardScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    onClick(event);
    event->accept();
}

void ChessBoardScene::selectPiece(const Chess::Piece* piece)
{
    isSelected_ = true;
    selectedPiece_ = piece;
    board_->select(selectedPiece_->pos(), game_);
}

void ChessBoardScene::deselectPiece()
{
    isSelected_ = false;
    selectedPiece_ = nullptr;
    board_->deselect();
}

bool ChessBoardScene::attemptMove(Chess::BP src, Chess::BP dst)
{
    const Chess::Move move = game_.move(src, dst);
    if (move.isValid())
    {
        emit moveSelected(move);
        return true;
    }
    return false;
}

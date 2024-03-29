#include "chessboardscene.h"
#include "chesswidget.h"
#include "chesspieceitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QMimeData>

ChessBoardScene::ChessBoardScene(const Chess::Game& game, QObject* parent):
    QGraphicsScene(parent), game_(game), board_(new ChessBoardItem(game))
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
    connect(board_, &ChessBoardItem::mousePress, this, &ChessBoardScene::onChessBoardClick);
    addItem(board_);

    for(int player = 0; player < NUM_PLAYERS; player++)
    {
        for(int id = 0; id < NUM_CHESS_PIECES; id++)
        {
            const Chess::Piece* ptr = game.piecePtr(player, id);
            pieces_[player][id].setChessPiece(ptr);
            connect(&pieces_[player][id], &ChessPieceItem::mousePress, this, &ChessBoardScene::onChessPieceClick);
            connect(&pieces_[player][id], &ChessPieceItem::mouseRelease, this, &ChessBoardScene::onChessPieceRelease);
            addItem(&pieces_[player][id]);
        }
    }
    updatePiecePositions();
}

void ChessBoardScene::updatePiecePositions()
{
    for(int player = 0; player < NUM_PLAYERS; player++)
    {
        for(int id = 0; id < NUM_CHESS_PIECES; id++)
        {
            pieces_[player][id].updatePos();
        }
    }
}

void ChessBoardScene::onReadyForNextMove()
{
    setPiecesMovable(game_.activePlayer());
}

void ChessBoardScene::onPerformMove()
{
    setPiecesMovable(false);
    board_->setCheckState(game_);
    updatePiecePositions();
}

Chess::BP ChessBoardScene::toChessBoardPosition(QPointF point)
{
    int x = point.x();
    int y = point.y();

    int i = (y / SQUARE_WIDTH);
    int j = (x / SQUARE_WIDTH);
    return Chess::BP(i, j);
}

void ChessBoardScene::onClick(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
    if (item == board_)
    {
        onChessBoardClick(event);
    }
    else if(auto pieceItem = qgraphicsitem_cast<ChessPieceItem*>(item))
    {
        onChessPieceClick(pieceItem);
        pieceItem->mousePressEvent(event);
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
            return;
        }
        else if (activePlayer == clickedPlayer)
        {
            selectPiece(clickedPiece);
        }
        else
        {
            const auto clickedPos = clickedPiece->pos();
            const auto selectedPiecePos = selectedPiece_->pos();
            attemptMove(selectedPiecePos, clickedPos);
        }
    }
    else if(activePlayer == clickedPlayer)
    {
        selectPiece(clickedPiece);
    }
}

void ChessBoardScene::onChessPieceRelease(QGraphicsSceneMouseEvent* event)
{
    const auto releasePos = toChessBoardPosition(event->scenePos());

    if (isSelected_)
    {
        const auto selectedPiecePos = selectedPiece_->pos();
        if (selectedPiecePos != releasePos)
        {
            attemptMove(selectedPiecePos, releasePos);
        }
        else if (!isRecentSelection_)
        {
            deselectPiece();
        }
    }
    isRecentSelection_ = false;
}

void ChessBoardScene::selectPiece(const Chess::Piece* piece)
{
    isSelected_ = true;
    isRecentSelection_ = true;
    selectedPiece_ = piece;
    board_->setSelectedState(selectedPiece_->pos(), game_);
}

void ChessBoardScene::deselectPiece()
{
    isSelected_ = false;
    selectedPiece_ = nullptr;
    board_->setDeselectedState();
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

void ChessBoardScene::setPiecesMovable(Chess::Player player)
{
    Chess::Player active = player;
    Chess::Player inactive = (player == Chess::Player::White ? Chess::Player::Black : Chess::Player::White);

    for(int pid=0; pid<NUM_CHESS_PIECES; pid++)
    {
        pieces_[active][pid].setFlag(QGraphicsItem::ItemIsMovable, true);
        pieces_[inactive][pid].setFlag(QGraphicsItem::ItemIsMovable, false);
    }
}

void ChessBoardScene::setPiecesMovable(bool movable)
{
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++)
    {
        pieces_[Chess::Player::White][pid].setFlag(QGraphicsItem::ItemIsMovable, movable);
        pieces_[Chess::Player::Black][pid].setFlag(QGraphicsItem::ItemIsMovable, movable);
    }
}

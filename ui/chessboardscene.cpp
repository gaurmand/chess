#include "chessboardscene.h"
#include "ui.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QMimeData>
#include <QCursor>

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
            connect(&pieces_[player][id], &ChessPieceItem::mouseMove, board_, &ChessBoardItem::updateHoverPos);
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

void ChessBoardScene::onPerformMove(const Chess::Move& move)
{
    setPiecesMovable(false);
    board_->updateState(game_, move);
    updatePiecePositions();
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
        const auto clickedPos = ui::sceneToBP(event->scenePos());
        const auto selectedPiecePos = selectedPiece_->pos();

        if (!attemptClickMove(selectedPiecePos, clickedPos))
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
            attemptClickMove(selectedPiecePos, clickedPos);
        }
    }
    else if(activePlayer == clickedPlayer)
    {
        selectPiece(clickedPiece);
    }
}

void ChessBoardScene::onChessPieceRelease(QGraphicsSceneMouseEvent* event)
{
    const auto releasePos = ui::sceneToBP(event->scenePos());

    if (isSelected_)
    {
        const auto selectedPiecePos = selectedPiece_->pos();
        if (selectedPiecePos != releasePos)
        {
            attemptDragMove(selectedPiecePos, releasePos);
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

bool ChessBoardScene::attemptClickMove(const Chess::BP& src, const Chess::BP& dst)
{
    if (!src.isValid() || !dst.isValid())
    {
        return false;;
    }

    const Chess::Move move = game_.move(src, dst);
    if (move.isValid())
    {
        // animate move
        ChessPieceItem* pieceItem = &pieces_[selectedPiece_->owner()][selectedPiece_->id()];
        QPropertyAnimation* animation = moveAnimation(pieceItem, move.dst());
        connect(animation, &QPropertyAnimation::finished, this, [&, move, animation]() {
            emit moveSelected(move);
            animation->deleteLater();
        });
        setPiecesMovable(false);
        animation->start();

        return true;
    }
    return false;
}

bool ChessBoardScene::attemptDragMove(const Chess::BP& src, const Chess::BP& dst)
{
    if (!src.isValid() || !dst.isValid())
    {
        return false;;
    }

    const Chess::Move move = game_.move(src, dst);
    if (move.isValid())
    {
        emit moveSelected(move);
        return true;
    }

    // animate returning piece
    ChessPieceItem* pieceItem = &pieces_[selectedPiece_->owner()][selectedPiece_->id()];
    QPropertyAnimation* animation = moveAnimation(pieceItem, src);
    connect(animation, &QPropertyAnimation::finished, this, [&, animation]() {
        setPiecesMovable(game_.activePlayer());
        animation->deleteLater();
    });
    setPiecesMovable(false);
    animation->start();

    return false;
}

QPropertyAnimation* ChessBoardScene::moveAnimation(ChessPieceItem* pieceItem, const Chess::BP& dst)
{
    auto res = new QPropertyAnimation(pieceItem, "pos");
    res->setEndValue(ui::BPToScene(dst));
    res->setEasingCurve(QEasingCurve::OutQuad);
    res->setDuration(250);
    return res;
}

void ChessBoardScene::setPiecesMovable(Chess::Player player)
{
    Chess::Player active = player;
    Chess::Player inactive = (player == Chess::Player::White ? Chess::Player::Black : Chess::Player::White);

    for(int pid=0; pid<NUM_CHESS_PIECES; pid++)
    {
        pieces_[active][pid].setFlag(QGraphicsItem::ItemIsMovable, true);
        pieces_[active][pid].setCursor(Qt::OpenHandCursor);

        pieces_[inactive][pid].setFlag(QGraphicsItem::ItemIsMovable, false);
        pieces_[inactive][pid].setCursor(Qt::ArrowCursor);

    }
}

void ChessBoardScene::setPiecesMovable(bool movable)
{
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++)
    {
        pieces_[Chess::Player::White][pid].setFlag(QGraphicsItem::ItemIsMovable, movable);
        pieces_[Chess::Player::Black][pid].setFlag(QGraphicsItem::ItemIsMovable, movable);
        pieces_[Chess::Player::White][pid].setCursor(movable ? Qt::OpenHandCursor : Qt::ArrowCursor);
        pieces_[Chess::Player::Black][pid].setCursor(movable ? Qt::OpenHandCursor : Qt::ArrowCursor);
    }
}

#include "chessboardscene.h"
#include "ui.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QCursor>
#include <QTimer>

ChessBoardScene::ChessBoardScene(std::vector<std::vector<ChessPieceItem*>>& pieces, QObject* parent):
    QGraphicsScene(parent),
    board_(new ChessBoardItem()),
    pieces_(std::move(pieces))
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
    addItem(board_);

    for(int player = 0; player < NUM_PLAYERS; player++)
    {
        for(int id = 0; id < NUM_CHESS_PIECES; id++)
        {
            addItem(pieces_[player][id]);
        }
    }
}

ChessBoardScene::~ChessBoardScene()
{
    for(int player = 0; player < NUM_PLAYERS; player++)
    {
        for(int id = 0; id < NUM_CHESS_PIECES; id++)
        {
            pieces_[player][id]->deleteLater();
        }
    }
    board_->deleteLater();
}

void ChessBoardScene::updatePieces()
{
    for(int player = 0; player < NUM_PLAYERS; player++)
    {
        for(int id = 0; id < NUM_CHESS_PIECES; id++)
        {
            pieces_[player][id]->updatePos();
        }
    }
}

void ChessBoardScene::enablePlayerMoveSelection(const Chess::Player active, const Chess::PlayerType type)
{
    if (type != Chess::PlayerType::Human)
    {
        return;
    }
    active_ = active;

    if (currAnimation_ && currAnimation_->state() == QAbstractAnimation::Running)
    {
        connect(currAnimation_, &QAbstractAnimation::finished, this, [=]() {
            enablePlayerMoveSelection(active, type);
        });
    }
    else
    {
        setPiecesMovable(active_);
    }
}

void ChessBoardScene::onMovePerformed(const Chess::Move& move)
{
    if (attemptedMoveType == MoveType::PieceClick || attemptedMoveType == MoveType::BoardClick)
    {
        // animate successful move
        ChessPieceItem* pieceItem = chessPieceItemAt(move.dst());
        currAnimation_ = moveAnimation(pieceItem, move.dst());
        connect(currAnimation_, &QPropertyAnimation::finished, this, [=]() {
            pieceItem->setZValue(0);
            deselect(false);
            setPiecesMovable(active_);
        });
        pieceItem->setZValue(1);
        currAnimation_->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        setPiecesMovable(active_);
    }
}

void ChessBoardScene::onMoveFailed(const Chess::Move& move)
{
    if (attemptedMoveType == MoveType::PieceDrag)
    {
        // animate returning piece
        ChessPieceItem* pieceItem = chessPieceItemAt(move.src());
        currAnimation_ = moveAnimation(pieceItem, move.src());
        connect(currAnimation_, &QPropertyAnimation::finished, this, [=]() {
            pieceItem->setZValue(0);
            setPiecesMovable(active_);
        });
        pieceItem->setZValue(1);
        currAnimation_->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else if (attemptedMoveType == MoveType::BoardClick)
    {
        deselect();
        setPiecesMovable(active_);
    }
}

void ChessBoardScene::reset()
{
    setPiecesMovable(false);
    deselect();
    active_ = Chess::Player::White;
}

void ChessBoardScene::disablePlayerMoveSelection()
{
    setPiecesMovable(false);
}

void ChessBoardScene::updateBoard(const ui::BPStates& states)
{
    if (currAnimation_ && currAnimation_->state() == QAbstractAnimation::Running)
    {
        connect(currAnimation_, &QAbstractAnimation::finished, this, [=](){
            updateBoard(states);
        });
    }
    else
    {
        board_->setBPStates(states);
        updatePieces();
    }
}

void ChessBoardScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mousePressEvent(event);
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
    const Chess::BP pressPos = ui::sceneToBP(event->scenePos());

    QTimer::singleShot(0, [=](){
        auto pieceItem = qgraphicsitem_cast<ChessPieceItem*>(item);
        if(pieceItem && (pieceItem->flags() & QGraphicsItem::ItemIsMovable))
        {
            onChessPiecePress(pieceItem);
        }
        else
        {
            onChessBoardPress(pressPos);
        }
    });
}

void ChessBoardScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
    const Chess::BP releasePos = ui::sceneToBP(event->scenePos());

    QGraphicsScene::mouseReleaseEvent(event);
    QTimer::singleShot(0, [=](){
        auto pieceItem = qgraphicsitem_cast<ChessPieceItem*>(item);
        if(pieceItem && (pieceItem->flags() & QGraphicsItem::ItemIsMovable))
        {
            onChessPieceRelease(releasePos);
        }
    });
}

void ChessBoardScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
    const QPointF lastPos = event->lastScenePos();

    QGraphicsScene::mouseMoveEvent(event);
    QTimer::singleShot(0, [=](){
        if(auto pieceItem = qgraphicsitem_cast<ChessPieceItem*>(item))
        {
            board_->updateHoverPos(ui::sceneToBP(lastPos));
        }
    });
}

void ChessBoardScene::onChessBoardPress(const Chess::BP& pressPos)
{
    if (isSelected_)
    {
        const auto selectedPiecePos = selectedPiece_->pos();
        selectMove(selectedPiecePos, pressPos, MoveType::BoardClick);
    }
}

void ChessBoardScene::onChessPiecePress(const ChessPieceItem* pieceItem)
{
    const Chess::Piece* clickedPiece = pieceItem->chessPiece();
    const Chess::Player clickedPlayer = clickedPiece->owner();

    if (isSelected_)
    {
        if (clickedPiece == selectedPiece_)
        {
            return;
        }
        else if (active_ == clickedPlayer)
        {
            selectPiece(clickedPiece);
        }
        else
        {
            const auto clickedPos = clickedPiece->pos();
            const auto selectedPiecePos = selectedPiece_->pos();
            selectMove(selectedPiecePos, clickedPos, MoveType::PieceClick);
        }
    }
    else if(active_ == clickedPlayer)
    {
        selectPiece(clickedPiece);
    }
}

void ChessBoardScene::onChessPieceRelease(const Chess::BP& releasePos)
{
    if (isSelected_)
    {
        const auto selectedPiecePos = selectedPiece_->pos();
        if (selectedPiecePos != releasePos)
        {
            selectMove(selectedPiecePos, releasePos, MoveType::PieceDrag);
        }
        else if (!isRecentSelection_)
        {
            deselect();
        }
        else
        {
            updatePieces();
        }
    }
    isRecentSelection_ = false;
}

void ChessBoardScene::selectPiece(const Chess::Piece* piece)
{
    isSelected_ = true;
    isRecentSelection_ = true;
    selectedPiece_ = piece;
    emit pieceSelected(selectedPiece_->pos());
}

void ChessBoardScene::deselect(const bool emitSignal)
{
    isSelected_ = false;
    selectedPiece_ = nullptr;

    if (emitSignal)
        emit deselected();
}

void ChessBoardScene::selectMove(const Chess::BP& src, const Chess::BP& dst, MoveType type)
{
    attemptedMoveType = type;
    setPiecesMovable(false);
    emit moveSelected(src, dst);
}

QPropertyAnimation* ChessBoardScene::moveAnimation(ChessPieceItem* pieceItem, const Chess::BP& dst)
{
    auto animation = new QPropertyAnimation(pieceItem, "pos");
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->setDuration(200);
    animation->setEndValue(ui::BPToScene(dst));
    return animation;
}

void ChessBoardScene::setPiecesMovable(Chess::Player player)
{
    Chess::Player active = player;
    Chess::Player inactive = (player == Chess::Player::White ? Chess::Player::Black : Chess::Player::White);

    for(int pid=0; pid<NUM_CHESS_PIECES; pid++)
    {
        pieces_[active][pid]->setFlag(QGraphicsItem::ItemIsMovable, true);
        pieces_[active][pid]->setCursor(Qt::OpenHandCursor);

        pieces_[inactive][pid]->setFlag(QGraphicsItem::ItemIsMovable, false);
        pieces_[inactive][pid]->setCursor(Qt::ArrowCursor);
    }
}

void ChessBoardScene::setPiecesMovable(bool movable)
{
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++)
    {
        pieces_[Chess::Player::White][pid]->setFlag(QGraphicsItem::ItemIsMovable, movable);
        pieces_[Chess::Player::Black][pid]->setFlag(QGraphicsItem::ItemIsMovable, movable);
        pieces_[Chess::Player::White][pid]->setCursor(movable ? Qt::OpenHandCursor : Qt::ArrowCursor);
        pieces_[Chess::Player::Black][pid]->setCursor(movable ? Qt::OpenHandCursor : Qt::ArrowCursor);
    }
}

ChessPieceItem* ChessBoardScene::chessPieceItemAt(const Chess::BP& pos)
{
    for(int player = 0; player < NUM_PLAYERS; player++)
    {
        for(int id = 0; id < NUM_CHESS_PIECES; id++)
        {
            ChessPieceItem* res = pieces_[player][id];
            if (res->chessPiece()->pos() == pos)
            {
                return res;
            }
        }
    }

    return nullptr;
}

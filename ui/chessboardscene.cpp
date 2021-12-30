#include "chessboardscene.h"
#include "ui.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QMimeData>
#include <QCursor>

ChessBoardScene::ChessBoardScene(std::vector<std::vector<ChessPieceItem*>>& pieces, QObject* parent):
    QGraphicsScene(parent),
    board_(new ChessBoardItem()),
    pieces_(std::move(pieces))
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
    connect(board_, &ChessBoardItem::mousePress, this, &ChessBoardScene::onChessBoardClick);
    addItem(board_);

    for(int player = 0; player < NUM_PLAYERS; player++)
    {
        for(int id = 0; id < NUM_CHESS_PIECES; id++)
        {
            connect(pieces_[player][id], &ChessPieceItem::mousePress, this, &ChessBoardScene::onChessPieceClick);
            connect(pieces_[player][id], &ChessPieceItem::mouseRelease, this, &ChessBoardScene::onChessPieceRelease);
            connect(pieces_[player][id], &ChessPieceItem::mouseMove, board_, &ChessBoardItem::updateHoverPos);
            addItem(pieces_[player][id]);
        }
    }

    activeAnimation_.setPropertyName("pos");
    activeAnimation_.setEasingCurve(QEasingCurve::OutQuad);
    activeAnimation_.setDuration(200);
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
    if (type == Chess::PlayerType::Human)
    {
        setPiecesMovable(active);
    }
    active_ = active;
}

void ChessBoardScene::onMovePerformed(const Chess::Move& move)
{
    if (attemptedMoveType == MoveType::PieceClick || attemptedMoveType == MoveType::BoardClick)
    {
        // animate successful move
        ChessPieceItem* pieceItem = pieces_[selectedPiece_->owner()][selectedPiece_->id()];
        setActiveAnimation(pieceItem, move.dst());
        connect(&activeAnimation_, &QPropertyAnimation::finished, this, [=]() {
            pieceItem->setZValue(0);
            deselect(false);
        });
        pieceItem->setZValue(1);
        activeAnimation_.start();
    }
}

void ChessBoardScene::onMoveFailed(const Chess::Move& move)
{
    if (attemptedMoveType == MoveType::PieceDrag)
    {
        // animate returning piece
        ChessPieceItem* pieceItem = pieces_[selectedPiece_->owner()][selectedPiece_->id()];
        setActiveAnimation(pieceItem, move.src());
        connect(&activeAnimation_, &QPropertyAnimation::finished, this, [=]() {
            pieceItem->setZValue(0);
        });
        pieceItem->setZValue(1);
        activeAnimation_.start();
    }
    else if (attemptedMoveType == MoveType::BoardClick)
    {
        deselect();
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
    if (activeAnimation_.state() == QAbstractAnimation::Running)
    {
        connect(&activeAnimation_, &QAbstractAnimation::finished, this, [&](){
            board_->setBPStates(states);
            updatePieces();
        });
    }
    else
    {
        board_->setBPStates(states);
        updatePieces();
    }
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
        selectMove(selectedPiecePos, clickedPos, MoveType::BoardClick);
    }
}

void ChessBoardScene::onChessPieceClick(const ChessPieceItem* pieceItem)
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

void ChessBoardScene::onChessPieceRelease(QGraphicsSceneMouseEvent* event)
{
    const auto releasePos = ui::sceneToBP(event->scenePos());

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
    emit moveSelected(src, dst);
}

void ChessBoardScene::setActiveAnimation(ChessPieceItem* pieceItem, const Chess::BP& dst)
{
    activeAnimation_.setTargetObject(pieceItem);
    activeAnimation_.setEndValue(ui::BPToScene(dst));
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

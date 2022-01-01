#include "chessboardscene.h"
#include "ui.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QCursor>
#include <QTimer>

ChessBoardScene::ChessBoardScene(): AbstractChessGame(Chess::PlayerType::Human, Chess::PlayerType::AI)
{
    board_ = new ChessBoardItem();
    addItem(board_);

    for(int player = 0; player < NUM_PLAYERS; player++)
    {
        pieces_.emplace_back();
        for(int id = 0; id < NUM_CHESS_PIECES; id++)
        {
            const Chess::Piece* ptr = game().piecePtr(player, id);
            auto item = new ChessPieceItem(ptr);
            pieces_[player].push_back(item);
            addItem(item);
        }
    }

    resetPiecePositions();
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

void ChessBoardScene::readyForHumanMove()
{
    enablePieceInteraction();
}

void ChessBoardScene::updateVisual()
{
    defaultStates_ = computeDefaultBPStates();
    if (previousMove().isValid())
    {
        ChessPieceItem* pieceItem = chessPieceItemAt(previousMove().dst());
        pieceItem->setZValue(1);
        animatePiece(pieceItem, previousMove().dst(), [=](){
            visualBoardUpdate();
            visualPiecesUpdate();
            resetPiecePositions();
            pieceItem->setZValue(0);
            nextMove();
        });
    }
    else
    {
        visualBoardUpdate();
        visualPiecesUpdate();
        resetPiecePositions();
        nextMove();
    }
}

void ChessBoardScene::finishGame()
{
    defaultStates_ = computeDefaultBPStates();

    assert(previousMove().isValid());
    ChessPieceItem* pieceItem = chessPieceItemAt(previousMove().dst());
    pieceItem->setZValue(1);
    animatePiece(pieceItem, previousMove().dst(), [=](){
        visualBoardUpdate();
        visualPiecesUpdate();
        resetPiecePositions();
        pieceItem->setZValue(0);
    });
}

void ChessBoardScene::enablePieceInteraction()
{
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++)
    {
        pieces_[activePlayer()][pid]->setFlag(QGraphicsItem::ItemIsMovable, true);
        pieces_[inactivePlayer()][pid]->setFlag(QGraphicsItem::ItemIsMovable, false);
        pieces_[activePlayer()][pid]->setCursor(Qt::OpenHandCursor);
        pieces_[inactivePlayer()][pid]->setCursor(Qt::ArrowCursor);
    }
}

void ChessBoardScene::disablePieceInteraction()
{
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++)
    {
        pieces_[activePlayer()][pid]->setFlag(QGraphicsItem::ItemIsMovable, false);
        pieces_[inactivePlayer()][pid]->setFlag(QGraphicsItem::ItemIsMovable, false);
        pieces_[activePlayer()][pid]->setCursor(Qt::ArrowCursor);
        pieces_[inactivePlayer()][pid]->setCursor(Qt::ArrowCursor);
    }
}

void ChessBoardScene::visualBoardUpdate(const Chess::BP& selected)
{
    if (selected.isValid())
    {
        board_->setBPStates(computeSelectedBPStates(selected));
    }
    else
    {
        board_->setBPStates(defaultStates_);
    }
}

void ChessBoardScene::visualPiecesUpdate()
{
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++)
    {
        pieces_[activePlayer()][pid]->updateVisual();
        pieces_[inactivePlayer()][pid]->updateVisual();
    }
}

void ChessBoardScene::resetPiecePositions()
{
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++)
    {
        pieces_[activePlayer()][pid]->updatePosition();
        pieces_[inactivePlayer()][pid]->updatePosition();
    }
}

bool ChessBoardScene::isPieceSelected()
{
    return selectedPiece_ != nullptr;
}

void ChessBoardScene::selectPiece(ChessPieceItem* piece)
{
    selectedPieceItem_ = piece;
    selectedPiece_ = piece->chessPiece();
    visualBoardUpdate(selectedPiece_->pos());
}

void ChessBoardScene::deselectPiece()
{
    selectedPieceItem_ = nullptr;
    selectedPiece_ = nullptr;
    visualBoardUpdate();
}

void ChessBoardScene::clearSelection()
{
    selectedPieceItem_ = nullptr;
    selectedPiece_ = nullptr;
}

void ChessBoardScene::animatePiece(ChessPieceItem* pieceItem, const Chess::BP& dst, std::function<void()> cb)
{
    auto animation = new QPropertyAnimation(pieceItem, "pos");
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->setDuration(200);
    animation->setEndValue(ui::BPToScene(dst));
    QObject::connect(animation, &QPropertyAnimation::finished, cb);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

ChessPieceItem* ChessBoardScene::chessPieceItemAt(const Chess::BP& pos)
{
    for(int player = 0; player < NUM_PLAYERS; player++)
    {
        for(int id = 0; id < NUM_CHESS_PIECES; id++)
        {
            ChessPieceItem* res = pieces_[player][id];
            const Chess::Piece* piece = pieces_[player][id]->chessPiece();
            if (res->chessPiece()->pos() == pos  && !piece->isCaptured())
            {
                return res;
            }
        }
    }
    return nullptr;
}

ui::BPStates ChessBoardScene::computeDefaultBPStates() const
{
    ui::BPStates states;
    std::for_each(states.begin(), states.end(), [](auto& row){ row.fill(ui::BPInfo::NONE); });

    // king square if in check
    if (game().isInCheck())
    {
        const Chess::BP checkPos = game().kingPosition(game().activePlayer());
        states[checkPos.row()][checkPos.col()] |= ui::BPInfo::CHECK;
    }

    // previous move
    if (previousMove().isValid())
    {
        const Chess::BP src = previousMove().src();
        states[src.row()][src.col()] |= ui::BPInfo::PREV_SRC;

        const Chess::BP dst = previousMove().dst();
        states[dst.row()][dst.col()] |= ui::BPInfo::PREV_DST;
    }

    return states;
}

ui::BPStates ChessBoardScene::computeSelectedBPStates(const Chess::BP selected) const
{
    ui::BPStates states = defaultStates_;

    // move source square
    states[selected.row()][selected.col()] |= ui::BPInfo::SOURCE;

    // move squares (normal and captures)
    const auto moves = game().moves(selected);
    for (const auto& move: moves)
    {
        const Chess::BP dst = move.dst();
        if (move.type() == Chess::MoveType::Capture)
        {
            states[dst.row()][dst.col()] |= ui::BPInfo::CAPTURE;
        }
        else
        {
            states[dst.row()][dst.col()] |= ui::BPInfo::NORMAL_MOVE;
        }
    }

    return states;
}

void InteractiveChessBoardScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mousePressEvent(event);
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
    const Chess::BP pressPos = ui::sceneToBP(event->scenePos());

    // Timer ensures code is triggered after QGraphicsItem receives event
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

void InteractiveChessBoardScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
    const Chess::BP releasePos = ui::sceneToBP(event->scenePos());

    // Timer ensures code is triggered after QGraphicsItem receives event
    QGraphicsScene::mouseReleaseEvent(event);
    QTimer::singleShot(0, [=](){
        auto pieceItem = qgraphicsitem_cast<ChessPieceItem*>(item);
        if(pieceItem && (pieceItem->flags() & QGraphicsItem::ItemIsMovable))
        {
            onChessPieceRelease(releasePos);
        }
    });
}
void InteractiveChessBoardScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
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

void InteractiveChessBoardScene::onChessBoardPress(const Chess::BP& pressPos)
{
    if (isPieceSelected())
    {
        const Chess::BP selectedPiecePos = selectedPiece_->pos();
        const Chess::BP& src = selectedPiecePos;
        const Chess::BP& dst = pressPos;

        if (game().isLegalMove(src, dst))
        {
            disablePieceInteraction();
            clearSelection();
            performMove(Chess::Move(src, dst));
        }
        else
        {
            deselectPiece();
        }
    }
}

void InteractiveChessBoardScene::onChessPiecePress(ChessPieceItem* pieceItem)
{
    const Chess::Piece* clickedPiece = pieceItem->chessPiece();
    const Chess::Player clickedPlayer = clickedPiece->owner();

    if (isPieceSelected())
    {
        if (clickedPiece == selectedPiece_)
        {
            return;
        }
        else if (activePlayer() == clickedPlayer)
        {
            isRecentSelection_ = true;
            selectPiece(pieceItem);
        }
        else
        {
            const Chess::BP src = selectedPiece_->pos();
            const Chess::BP dst = clickedPiece->pos();

            if (game().isLegalMove(src, dst))
            {
                disablePieceInteraction();
                clearSelection();
                performMove(Chess::Move(src, dst));
            }
            else
            {
                deselectPiece();
            }
        }
    }
    else if(activePlayer() == clickedPlayer)
    {
        isRecentSelection_ = true;
        selectPiece(pieceItem);
    }
}

void InteractiveChessBoardScene::onChessPieceRelease(const Chess::BP& releasePos)
{
    if (isPieceSelected())
    {
        const auto selectedPiecePos = selectedPiece_->pos();
        if (selectedPiecePos != releasePos)
        {
            const Chess::BP& src = selectedPiecePos;
            const Chess::BP& dst = releasePos;

            if (game().isLegalMove(src, dst))
            {
                disablePieceInteraction();
                clearSelection();
                performMove(Chess::Move(src, dst));
            }
            else
            {
                selectedPieceItem_->setZValue(1);
                animatePiece(selectedPieceItem_, selectedPiece_->pos(), [&](){
                    enablePieceInteraction();
                    selectedPieceItem_->setZValue(0);
                });
            }
        }
        else
        {
            if (!isRecentSelection_)
            {
                deselectPiece();
            }
            resetPiecePositions();
        }
    }
    isRecentSelection_ = false;
}

#include "chessboarditem.h"
#include "ui.h"
#include "chess/move.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>
#include <QWidget>

ChessBoardItem::ChessBoardItem(const Chess::Game& game)
{
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
    std::for_each(states_.begin(), states_.end(), [](auto& row){ row.fill(SquareState::NONE); });
}

QRectF ChessBoardItem::boundingRect() const
{
    return QRectF(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
}

void ChessBoardItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    drawBoard(painter);
}

void drawAxes(QPainter *painter) {
    painter->setPen(Qt::black);
    painter->drawLine(QLineF(0,0,1000,0));
    painter->drawLine(QLineF(0,0,-1000,0));
    painter->drawLine(QLineF(0,0,0,1000));
    painter->drawLine(QLineF(0,0,0,-1000));
}

void drawBounds(QPainter *painter) {
    painter->setPen(Qt::black);
    painter->drawLine(QLineF(0,0,BOARD_WIDTH,0));
    painter->drawLine(QLineF(BOARD_WIDTH,0,BOARD_WIDTH,BOARD_HEIGHT));
    painter->drawLine(QLineF(BOARD_WIDTH,BOARD_HEIGHT,0,BOARD_HEIGHT));
    painter->drawLine(QLineF(0,BOARD_HEIGHT,0,0));
}

void ChessBoardItem::drawSquare(int i, int j, QPainter *painter)
{
    const bool isMouseOver = Chess::BP(i,j) == hoverPos_;
    const auto drawMouseOverMove = [&]()
    {
        static const int width = 5;
        static const QRect outline = ui::kBoardSquareRect.marginsRemoved(QMargins(2, 2, 2, 2));
        QPen pen(ui::colour::kBoardMoveMouseOver);
        pen.setWidth(width);

        painter->save();
        painter->setPen(pen);
        painter->drawRect(outline);
        painter->restore();
    };

    painter->save();
    painter->translate(j*SQUARE_WIDTH, i*SQUARE_WIDTH);

    if((i+j) % 2 == 0)
    {
        painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardWhite);
    }
    else
    {
        painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardBlack);
    }

    const SquareState state = states_[i][j];
    switch(state)
    {
        case SquareState::CHECK:
            painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardCheck);
            break;
        case SquareState::SOURCE:
            painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardSource);
            break;
        case SquareState::CAPTURE:
            painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardCapture);
            if (isMouseOver)
            {
                drawMouseOverMove();
            }
            break;
        case SquareState::NORMAL_MOVE:
        {
            static const QPointF center(SQUARE_WIDTH/2, SQUARE_WIDTH/2);
            static const int radius = 15;

            painter->save();
            painter->setPen(Qt::NoPen);
            painter->setBrush(ui::colour::kBoardCapture);
            painter->drawEllipse(center, radius,  radius);
            painter->restore();

            if (isMouseOver)
            {
                drawMouseOverMove();
            }
            break;
        }
        case SquareState::NONE:
            break;
    }

    painter->restore();
}

void ChessBoardItem::drawBoard(QPainter* painter)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            drawSquare(i, j, painter);
        }
    }
}

void ChessBoardItem::setSelectedState(const Chess::BP& src, const Chess::Game& game)
{
    // initialize all states to normal
    std::for_each(states_.begin(), states_.end(), [](auto& row){ row.fill(SquareState::NONE); });

    // move source square
    states_[src.row()][src.col()] = SquareState::SOURCE;

    // king square if in check
    if (isInCheck_)
    {
        states_[checkPos_.row()][checkPos_.col()] = SquareState::CHECK;
    }

    // move squares (normal and captures)
    const auto moves = game.moves(src);
    for (const auto& move: moves)
    {
        const Chess::BP dst = move.dst();
        if (move.type() == Chess::MoveType::Capture)
        {
            states_[dst.row()][dst.col()] = SquareState::CAPTURE;
        }
        else
        {
            states_[dst.row()][dst.col()] = SquareState::NORMAL_MOVE;
        }
    }

    update();
}

void ChessBoardItem::setDeselectedState()
{
    std::for_each(states_.begin(), states_.end(), [](auto& row){ row.fill(SquareState::NONE); });
    if (isInCheck_)
    {
        states_[checkPos_.row()][checkPos_.col()] = SquareState::CHECK;
    }
    update();
}

void ChessBoardItem::setCheckState(const Chess::Game& game)
{
    isInCheck_ = game.isInCheck();
    if (isInCheck_)
    {
        checkPos_ = game.kingPosition(game.activePlayer());
    }
    setDeselectedState();
}

void ChessBoardItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mousePressEvent(event);
    emit mousePress(event);
}

void ChessBoardItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    updateHoverPos(ui::sceneToBP(event->lastScenePos()));
}

void ChessBoardItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    updateHoverPos(Chess::BP());
}

void ChessBoardItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    updateHoverPos(ui::sceneToBP(event->lastScenePos()));
}

void ChessBoardItem::updateHoverPos(const Chess::BP& pos)
{
    hoverPos_ = pos;
    update();
}


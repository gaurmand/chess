#include "chessboarditem.h"
#include "ui.h"
#include "chess/move.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>
#include <QWidget>

ChessBoardItem::ChessBoardItem()
{
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
    std::for_each(states_.begin(), states_.end(), [](auto& row){ row.fill(ui::BPInfo::NONE); });
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

    const ui::BPState state = states_[i][j];

    // Paint position w/ your piece
    if (state & ui::BPInfo::CHECK)
    {
        painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardCheck);
    }
    else if (state & ui::BPInfo::SOURCE)
    {
        painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardSource);
    }

    // Paint position w/ enemy piece
    if (state & ui::BPInfo::CAPTURE)
    {
        painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardCapture);
        if (isMouseOver)
        {
            drawMouseOverMove();
        }
    }
    else if (state & ui::BPInfo::PREV_DST)
    {
        painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardPrevDst);
    }

    // Paint position w/ no piece
    if (state & ui::BPInfo::PREV_SRC)
    {
        painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardPrevSrc);
    }
    if (state & ui::BPInfo::NORMAL_MOVE)
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

void ChessBoardItem::setBPStates(const ui::BPStates& states)
{
    states_ = states;
    update();
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


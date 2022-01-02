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
    return ui::kBoardRect;
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
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->translate(j*ui::kBoardSquareRect.width(), i*ui::kBoardSquareRect.height());

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
        const QPointF center = ui::kBoardSquareRect.center();
        const double radius = ui::kBoardSquareRect.width() / 5;

        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(ui::colour::kBoardCapture);
        painter->drawEllipse(center, radius,  radius);
        painter->restore();
    }

    painter->restore();
}

void ChessBoardItem::drawBoard(QPainter* painter)
{
    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            drawSquare(i, j, painter);
        }
    }

    const auto drawOutline = [&](int i, int j)
    {
        const double width = ui::kBoardSquareRect.width() / 20.0;
        QPen pen(ui::colour::kBoardMoveMouseOver);
        pen.setWidth(width);

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->translate(j*ui::kBoardSquareRect.width(), i*ui::kBoardSquareRect.height());
        painter->setPen(pen);
        painter->drawRect(ui::kBoardSquareRect);
        painter->restore();
    };

    if (!hoverPos_.isValid())
    {
        return;
    }

    const int i = hoverPos_.row(), j = hoverPos_.col();
    const ui::BPState state = states_[i][j];
    if (state & ui::BPInfo::CAPTURE || state & ui::BPInfo::NORMAL_MOVE)
    {
        drawOutline(i, j);
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


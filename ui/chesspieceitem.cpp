#include "chesspieceitem.h"
#include "ui.h"

#include <QPainter>
#include <QGraphicsSceneEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QString>

#include <iostream>

ChessPieceItem::ChessPieceItem()
{
    setZValue(0);
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF ChessPieceItem::boundingRect() const
{
    return ui::kBoardSquareRect;
}

void ChessPieceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawPixmap(ui::kBoardSquareRect, ui::piecePixmap(piece_->owner(), piece_->type()));
    painter->restore();
}

void ChessPieceItem::updatePos()
{

    const int x = piece_->pos().col()*SQUARE_WIDTH;
    const int y = piece_->pos().row()*SQUARE_WIDTH;
    setPos(QPointF(x,y));

    if (piece_->isCaptured())
    {
        setVisible(false);
    }
    update();
}

void ChessPieceItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        setZValue(1);
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsItem::mousePressEvent(event);
    emit mousePress(this);
}

void ChessPieceItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        setZValue(0);
        setCursor(Qt::OpenHandCursor);
    }
    QGraphicsItem::mouseReleaseEvent(event);
    emit mouseRelease(event);
    updatePos();
}

void ChessPieceItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseMoveEvent(event);

    emit mouseMove(ui::sceneToBP(event->lastScenePos()));
}


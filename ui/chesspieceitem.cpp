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
    setCursor(Qt::OpenHandCursor);
}

QRectF ChessPieceItem::boundingRect() const
{
    return ui::kBoardSquareRect;
}

void ChessPieceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->drawPixmap(ui::kBoardSquareRect, ui::piecePixmap(piece_->owner(), piece_->type()));
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
    setZValue(1);
    QGraphicsItem::mousePressEvent(event);
    setCursor(Qt::ClosedHandCursor);
    emit mousePress(this);
}

void ChessPieceItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    setZValue(0);
    QGraphicsItem::mouseReleaseEvent(event);
    setCursor(Qt::OpenHandCursor);
    emit mouseRelease(event);
    updatePos();
}

#include "chesspieceitem.h"
#include "ui.h"

#include <QGraphicsSceneEvent>
#include <QApplication>

#include <iostream>

ChessPieceItem::ChessPieceItem(const Chess::Piece* piece): piece_(piece)
{
    setZValue(0);
    setAcceptedMouseButtons(Qt::LeftButton);
    setShapeMode(ShapeMode::MaskShape);
    type_ = piece_->type();
    setPixmap(ui::piecePixmap(piece_->owner(), type_));
}

void ChessPieceItem::updatePos()
{
    if (piece_->isCaptured())
    {
        setVisible(false);
        return;
    }

    const QPointF expectedPos = ui::BPToScene(piece_->pos());
    if (pos() != expectedPos)
    {
        setPos(expectedPos);
    }
    else if (type_ != piece_->type())
    {
        type_ = piece_->type();
        setPixmap(ui::piecePixmap(piece_->owner(), type_));
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
}

void ChessPieceItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseMoveEvent(event);

    emit mouseMove(ui::sceneToBP(event->lastScenePos()));
}


#include <QPainter>
#include <QGraphicsSceneEvent>
#include <iostream>
#include "chesswidget.h"
#include "chesspieceqgraphicsitem.h"

ChessPieceQGraphicsItem::ChessPieceQGraphicsItem(ChessWidget *cw, const Chess::Piece& pc)
    : chessWidget_(cw), piece_(pc)
{
    setZValue(0);
    setFlag(ItemIsMovable);
    setBoardPosition();
    setPixmap();
}

QRectF ChessPieceQGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, SQUARE_WIDTH, SQUARE_WIDTH);
}

void ChessPieceQGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->drawPixmap(QRect(0, 0, SQUARE_WIDTH, SQUARE_WIDTH), *piecePixmap_);
}

void ChessPieceQGraphicsItem::setBoardPosition()
{
    Chess::BP pos = piece_.pos();
    setPos(pos.col()*SQUARE_WIDTH, pos.row()*SQUARE_WIDTH);
}

void ChessPieceQGraphicsItem::setPixmap()
{
    piecePixmap_ = chessWidget_->getPiecePixmap(piece_.type(), piece_.owner());
}

void ChessPieceQGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    setZValue(1);
    chessWidget_->chessPieceItemMousePress(&piece_);

    QGraphicsItem::mousePressEvent(event);
}

void ChessPieceQGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    setZValue(0);
    chessWidget_->chessPieceItemMouseRelease(&piece_, event->scenePos());
    setBoardPosition();

    if (piece_.isCaptured())
    {
        setVisible(false);
    }
    update();

    QGraphicsItem::mouseReleaseEvent(event);
}

//void ChessPieceQGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
//{
//    std::cout << piece->toString() << " mouse move" << std::endl;
//    QGraphicsItem::mouseMoveEvent(event);
//}

//void ChessPieceQGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
//{
//    std::cout << piece->toString() << " mouse dbl click" << std::endl;
//    QGraphicsItem::mouseDoubleClickEvent(event);
//}



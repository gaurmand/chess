#include <QPainter>
#include "chesswidget.h"
#include "chesspieceqgraphicsitem.h"
#include "chesspiece.h"

ChessPieceQGraphicsItem::ChessPieceQGraphicsItem(ChessWidget *cw,  ChessPiece* pc)
    : chessWidget(cw), piece(pc)
{
    setZValue(1);
    setBoardPosition();
    setPixmap();
}

QRectF ChessPieceQGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, SQUARE_WIDTH, SQUARE_WIDTH);
}

void ChessPieceQGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->drawPixmap(QRect(0, 0, SQUARE_WIDTH, SQUARE_WIDTH), *piecePixmap);
}

void ChessPieceQGraphicsItem::setBoardPosition()
{
    IBP pos = piece->getIBP();
    setPos(pos.col*SQUARE_WIDTH, pos.row*SQUARE_WIDTH);
}

void ChessPieceQGraphicsItem::setPixmap(){
    piecePixmap = chessWidget->getPiecePixmap(piece->getType(), piece->getOwner());
}

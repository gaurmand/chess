#include <QPainter>
#include <QGraphicsSceneEvent>
#include <iostream>
#include "chesswidget.h"
#include "chesspieceqgraphicsitem.h"

const QPixmap& getPixmap(Chess::Player player, Chess::PieceType type)

{
    static const QPixmap whiteKing(":/images/w0");
    static const QPixmap whiteQueen(":/images/w1");
    static const QPixmap whiteRook(":/images/w2");
    static const QPixmap whiteBishop(":/images/w3");
    static const QPixmap whiteKnight(":/images/w4");
    static const QPixmap whitePawn(":/images/w5");

    static const QPixmap blackKing(":/images/b0");
    static const QPixmap blackQueen(":/images/b1");
    static const QPixmap blackRook(":/images/b2");
    static const QPixmap blackBishop(":/images/b3");
    static const QPixmap blackKnight(":/images/b4");
    static const QPixmap blackPawn(":/images/b5");

    switch (type)
    {
        case Chess::PieceType::King:
            return player == Chess::Player::White ? whiteKing : blackKing;
        case Chess::PieceType::Queen:
            return player == Chess::Player::White ? whiteQueen : blackQueen;
        case Chess::PieceType::Rook:
            return player == Chess::Player::White ? whiteRook : blackRook;
        case Chess::PieceType::Bishop:
            return player == Chess::Player::White ? whiteBishop : blackBishop;
        case Chess::PieceType::Knight:
            return player == Chess::Player::White ? whiteKnight : blackKnight;
        case Chess::PieceType::Pawn:
            return player == Chess::Player::White ? whitePawn : blackPawn;
        default:
            throw 1;
    }
}


ChessPieceQGraphicsItem::ChessPieceQGraphicsItem(ChessWidget *cw, const Chess::Piece& pc)
    : chessWidget_(cw), piece_(pc)
{
    setZValue(0);
    setFlag(ItemIsMovable);
    setBoardPosition();
}

QRectF ChessPieceQGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, SQUARE_WIDTH, SQUARE_WIDTH);
}

void ChessPieceQGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->drawPixmap(QRect(0, 0, SQUARE_WIDTH, SQUARE_WIDTH), getPixmap(piece_.owner(), piece_.type()));
}

void ChessPieceQGraphicsItem::setBoardPosition()
{
    Chess::BP pos = piece_.pos();
    setPos(pos.col()*SQUARE_WIDTH, pos.row()*SQUARE_WIDTH);
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



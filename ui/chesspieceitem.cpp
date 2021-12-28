#include "chesswidget.h"
#include "chesspieceitem.h"
#include "chessboardscene.h"

#include <QPainter>
#include <QGraphicsSceneEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QString>

#include <iostream>

static const QRect kSquare = QRect(0, 0, SQUARE_WIDTH, SQUARE_WIDTH);

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


ChessPieceItem::ChessPieceItem()
{
    setZValue(0);
    setAcceptedMouseButtons(Qt::LeftButton);
    setCursor(Qt::OpenHandCursor);
}

QRectF ChessPieceItem::boundingRect() const
{
    return kSquare;
}

void ChessPieceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->drawPixmap(kSquare, getPixmap(piece_->owner(), piece_->type()));
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

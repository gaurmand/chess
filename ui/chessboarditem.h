#ifndef CHESSBOARDITEM_H
#define CHESSBOARDITEM_H

#include "chess/game.h"
#include "chesspieceitem.h"

#include <QGraphicsObject>

enum class SquareState{NONE, SOURCE, NORMAL_MOVE, CAPTURE, CHECK};

class ChessBoardItem : public QGraphicsObject
{
    Q_OBJECT
public:
    ChessBoardItem(const Chess::Game& game);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // Enable the use of qgraphicsitem_cast
    enum { Type = UserType + 1 };
    int type() const override{ return Type; }

public slots:
    void setSelectedState(const Chess::BP& src, const Chess::Game& game);
    void setDeselectedState();
    void setCheckState(const Chess::Game& game);

signals:
    void mousePress(QGraphicsSceneMouseEvent* event);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void drawSquare(int i, int j, QPainter *painter);
    void drawBoard(QPainter *painter);

    bool isInCheck_ = false;
    Chess::BP checkPos_;
    std::array<std::array<SquareState, NUM_COLS>, NUM_ROWS> states_;
};

#endif // CHESSBOARDITEM_H

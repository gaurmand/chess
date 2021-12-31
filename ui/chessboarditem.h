#ifndef CHESSBOARDITEM_H
#define CHESSBOARDITEM_H

#include "ui.h"

#include <QGraphicsObject>

class ChessBoardItem : public QGraphicsObject
{
    Q_OBJECT
public:
    ChessBoardItem();
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    // Enable the use of qgraphicsitem_cast
    enum { Type = UserType + 1 };
    int type() const override{ return Type; }

public slots:
    void setBPStates(const ui::BPStates& states);
    void updateHoverPos(const Chess::BP& pos);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    void drawSquare(int i, int j, QPainter* painter);
    void drawBoard(QPainter *painter);

    ui::BPStates states_;
    Chess::BP hoverPos_;
};

#endif // CHESSBOARDITEM_H

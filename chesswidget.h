#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include <QGraphicsView>
#include <QPixmap>
#include "chessboardqgraphicsitem.h"
#include "chesspieceqgraphicsitem.h"

class ChessWidget : public QGraphicsView
{
    Q_OBJECT
public:
    ChessWidget(QWidget *parent = nullptr);

private:
    ChessBoardQGraphicsItem chessBoard;
    ChessPieceQGraphicsItem pieces[2][16];
    QPixmap piecePixmaps[2][6];
};

#endif // CHESSWIDGET_H

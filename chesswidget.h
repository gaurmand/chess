#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include <QGraphicsView>
#include <QPixmap>
#include <vector>
#include "constants.h"
#include "chessgame.h"
#include "chessboardqgraphicsitem.h"
#include "chesspieceqgraphicsitem.h"

class ChessWidget : public QGraphicsView
{
    Q_OBJECT
public:
    ChessWidget(QWidget *parent = nullptr);
    ~ChessWidget();
    void resizeEvent(QResizeEvent *event);
    void setInitialBoardState();
    QPixmap* getPiecePixmap(PieceType type, Player player);

private:
    ChessGame game;
    ChessBoardQGraphicsItem* chessBoard;
    ChessPieceQGraphicsItem* pieces[NUM_PLAYERS][NUM_CHESS_PIECES];
    QPixmap* piecePixmaps[NUM_PLAYERS][NUM_CHESS_PIECE_TYPES];

    QGraphicsScene* scene;
};

#endif // CHESSWIDGET_H

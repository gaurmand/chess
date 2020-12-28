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
    void resizeEvent(QResizeEvent *event) override;

    void setInitialBoardState();
    QPixmap* getPiecePixmap(PieceType type, Player player);

    void newGame(bool isWhiteAI);
    void nextTurn();
    ChessMove selectMove();
    ChessMove playerSelectMove();
    ChessMove AISelectMove();
    bool performMove(ChessMove move);

private:
    ChessGame game;
    bool isWhiteAI;

    ChessBoardQGraphicsItem* chessBoard;
    ChessPieceQGraphicsItem* pieces[NUM_PLAYERS][NUM_CHESS_PIECES];
    QPixmap* piecePixmaps[NUM_PLAYERS][NUM_CHESS_PIECE_TYPES];
    QGraphicsScene* scene;
};

#endif // CHESSWIDGET_H

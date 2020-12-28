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

    bool isReadyToDisplayMoves();
    void setReadyToDisplayMoves();
    void setUnreadyToDisplayMoves();

    bool isPieceSelected();
    PieceID getSelectedPiece();
    void setSelectedPiece(PieceID pid);
    void clearSelectedPiece();

    BGState getBGState(int i, int j);
    void updateChessBoard();

    Player getActivePlayer();


protected:
    void computeBoardGraphicalStates();

private:
    ChessGame game;
    bool isWhiteAI;
    bool readyToDisplayMoves = false;
    bool pieceSelected = false;
    PieceID selectedPiece;

    ChessBoardQGraphicsItem* chessBoard;
    BGState boardGraphicalState[NUM_CHESS_PIECES][NUM_ROWS][NUM_COLS];
    ChessPieceQGraphicsItem* pieces[NUM_PLAYERS][NUM_CHESS_PIECES];
    QPixmap* piecePixmaps[NUM_PLAYERS][NUM_CHESS_PIECE_TYPES];
    QGraphicsScene* scene;

};

#endif // CHESSWIDGET_H

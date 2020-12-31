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
    void mousePressEvent(QMouseEvent* event) override;
//    void mouseReleaseEvent(QMouseEvent* event) override;
    void chessPieceItemMousePress(ChessPiece* piece);
    void chessPieceItemMouseRelease(ChessPiece* piece, QPointF point);
    void chessBoardItemMousePress(IBP pos);
    void selectPiece(ChessPiece* piece);
    void deselectPiece();
    static IBP getChessboardPosition(QPointF point);
    static bool isClickInChessBoard(QPointF point);
    static bool isSamePosition(IBP p1, IBP p2);

    void setInitialBoardState();
    QPixmap* getPiecePixmap(PieceType type, Player player);

    void newGame();
    void startTurn();
    void completeTurn(ChessMove move);
    void playerTurn(ChessMove move);
    void AITurn(ChessMove move);

    bool isReadyToDisplayMoves();
    void setReadyToDisplayMoves();
    void setUnreadyToDisplayMoves();

    bool isPieceSelected();
    ChessPiece* getSelectedPiece();
    void setSelectedPiece(PieceID pid);
    void clearSelectedPiece();

    BGState getBGState(int i, int j);

protected:
    void computeBoardGraphicalStates();
    ChessMove getPlayerSelectedMove(ChessPiece* piece, IBP moveDst);
    void setPiecesMovable(Player player);
    void setAllPiecesUnmovable();
    void updatePieces();

private:
    ChessGame game;
    PlayerType playerType[NUM_PLAYERS] = {PlayerType::HUMAN, PlayerType::HUMAN};

    bool isRecentSelection = false;
    bool readyToDisplayMoves = false;
    bool pieceSelected = false;
    ChessPiece* selectedPiece;

    ChessBoardQGraphicsItem* chessBoard;
    BGState boardGraphicalState[NUM_CHESS_PIECES][NUM_ROWS][NUM_COLS];
    ChessPieceQGraphicsItem* pieces[NUM_PLAYERS][NUM_CHESS_PIECES];
    QPixmap* piecePixmaps[NUM_PLAYERS][NUM_CHESS_PIECE_TYPES];
    QGraphicsScene* scene;

};

#endif // CHESSWIDGET_H

#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include "chess/game.h"
#include "chessboardqgraphicsitem.h"
#include "chesspieceqgraphicsitem.h"

#include <QGraphicsView>
#include <QPixmap>
#include <QMessageBox>
#include <vector>

enum class PlayerType{HUMAN, AI};

#define SQUARE_WIDTH 90
#define BOARD_WIDTH SQUARE_WIDTH*NUM_ROWS
#define BOARD_HEIGHT SQUARE_WIDTH*NUM_COLS

class ChessWidget : public QGraphicsView
{
    Q_OBJECT
public:
    ChessWidget(QWidget *parent = nullptr);
    ~ChessWidget();
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
//    void mouseReleaseEvent(QMouseEvent* event) override;
    void chessPieceItemMousePress(const Chess::Piece* piece);
    void chessPieceItemMouseRelease(const Chess::Piece* piece, QPointF point);
    void chessBoardItemMousePress(Chess::BP pos);
    void selectPiece(const Chess::Piece* piece);
    void deselectPiece();
    static Chess::BP getChessboardPosition(QPointF point);
    static bool isClickInChessBoard(QPointF point);
    static bool isSamePosition(Chess::BP p1, Chess::BP p2);

    void setInitialBoardState();

    void newGame();
    void startTurn();
    void completeTurn(Chess::Move move);
    void playerTurn(Chess::Move move);
    void AITurn(Chess::Move move);

    bool isPieceSelected();
    const Chess::Piece* getSelectedPiece();
//    void setSelectedPiece(PieceID pid);
    void clearSelectedPiece();

protected:
    Chess::Move getPlayerSelectedMove(const Chess::Piece* piece, Chess::BP dst);
    void setPiecesMovable(Chess::Player player);
    void setAllPiecesUnmovable();
    void updatePieces();

private:
    Chess::Game game_;
    PlayerType playerType[NUM_PLAYERS] = {PlayerType::HUMAN, PlayerType::HUMAN};

    bool isRecentSelection = false;
    bool pieceSelected = false;
    const Chess::Piece* selectedPiece_;

    ChessBoardQGraphicsItem* chessBoard;
    ChessPieceQGraphicsItem* pieces[NUM_PLAYERS][NUM_CHESS_PIECES];
    QGraphicsScene* scene;
    QMessageBox gameEndBox;
};

#endif // CHESSWIDGET_H

#ifndef CHESSBOARDSCENE_H
#define CHESSBOARDSCENE_H

#include "chessboarditem.h"
#include "chesspieceitem.h"
#include "chess/game.h"
#include "ui.h"

#include <QGraphicsScene>
#include <QPropertyAnimation>

class ChessBoardScene: public QGraphicsScene
{
    Q_OBJECT
public:
    ChessBoardScene(const Chess::Game& game, QObject* parent = nullptr);

    enum class MoveType {Invalid, BoardClick, PieceClick, PieceDrag};

    void onChessBoardClick(QGraphicsSceneMouseEvent* event);
    void onChessPieceClick(const ChessPieceItem* pieceItem);
    void onChessPieceRelease(QGraphicsSceneMouseEvent* event);

public slots:
    void enablePlayerMoveSelection(const Chess::Player active, const Chess::PlayerType type);
    void onMovePerformed(const Chess::Move& move);
    void onMoveFailed(const Chess::Move& move);
    void reset();
    void disablePlayerMoveSelection();
    void updateBoard(const ui::BPStates& states);

signals:
    void moveSelected(const Chess::BP& src, const Chess::BP& dst);
    void pieceSelected(const Chess::BP& pos);
    void deselected();

private:
    void onClick(QGraphicsSceneMouseEvent* event);

    void selectMove(const Chess::BP& src, const Chess::BP& dst, MoveType type);
    MoveType attemptedMoveType = MoveType::Invalid;

    void selectPiece(const Chess::Piece* piece);
    void deselect(const bool emitSignal = true);

    void updatePieces();
    void setPiecesMovable(Chess::Player player);
    void setPiecesMovable(bool movable);

    void setActiveAnimation(ChessPieceItem* pieceItem, const Chess::BP& dst);
    QPropertyAnimation activeAnimation_;

    bool isSelected_ = false;
    const Chess::Piece* selectedPiece_ = nullptr;
    bool isRecentSelection_ = false;

    Chess::Player active_;
    ChessBoardItem* board_;
    std::array<std::array<ChessPieceItem, NUM_CHESS_PIECES>, NUM_PLAYERS> pieces_;
};

#endif // CHESSBOARDSCENE_H

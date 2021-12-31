#ifndef CHESSBOARDSCENE_H
#define CHESSBOARDSCENE_H

#include "chessboarditem.h"
#include "chesspieceitem.h"
#include "ui.h"

#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QPointer>

class ChessBoardScene: public QGraphicsScene
{
    Q_OBJECT
public:
    ChessBoardScene(std::vector<std::vector<ChessPieceItem*>>& pieces, QObject* parent = nullptr);
    ~ChessBoardScene();

    enum class MoveType {Invalid, BoardClick, PieceClick, PieceDrag};

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

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void onChessBoardPress(const Chess::BP& pressPos);
    void onChessPiecePress(const ChessPieceItem* pieceItem);
    void onChessPieceRelease(const Chess::BP& releasePos);

    void selectMove(const Chess::BP& src, const Chess::BP& dst, MoveType type);
    MoveType attemptedMoveType = MoveType::Invalid;

    void selectPiece(const Chess::Piece* piece);
    void deselect(const bool emitSignal = true);

    void updatePieces();
    void setPiecesMovable(Chess::Player player);
    void setPiecesMovable(bool movable);

    QPropertyAnimation* moveAnimation(ChessPieceItem* pieceItem, const Chess::BP& dst);
    QPointer<QPropertyAnimation> currAnimation_ = nullptr;

    bool isSelected_ = false;
    const Chess::Piece* selectedPiece_ = nullptr;
    bool isRecentSelection_ = false;

    Chess::Player active_;
    ChessBoardItem* board_;
    std::vector<std::vector<ChessPieceItem*>> pieces_;
};

#endif // CHESSBOARDSCENE_H

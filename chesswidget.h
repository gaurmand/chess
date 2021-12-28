#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include "chess/game.h"
#include "chessboarditem.h"
#include "chesspieceitem.h"

#include <QGraphicsView>
#include <QPixmap>
#include <QMessageBox>
#include <vector>

#define SQUARE_WIDTH 90
#define BOARD_WIDTH SQUARE_WIDTH*NUM_ROWS
#define BOARD_HEIGHT SQUARE_WIDTH*NUM_COLS

enum class PlayerType{HUMAN, AI};

class ChessWidget : public QGraphicsView
{
    Q_OBJECT
public:
    ChessWidget(Chess::Game& game, QWidget *parent = nullptr);
    ~ChessWidget() = default;
    void resizeEvent(QResizeEvent *event) override;

    void newGame();
    void startTurn();
    void completeTurn(Chess::Move move);

signals:
    void readyToReceiveUserMove();
    void movePerformed();

private:
    Chess::Game& game_;
    PlayerType playerType[NUM_PLAYERS] = {PlayerType::HUMAN, PlayerType::HUMAN};
    QMessageBox gameEndBox;
};

#endif // CHESSWIDGET_H

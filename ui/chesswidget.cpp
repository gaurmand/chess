#include "chesswidget.h"

#include <QFrame>
#include <QMouseEvent>
#include <QGraphicsItem>

#include <iostream>


ChessWidget::ChessWidget(Chess::Game& game, QWidget *parent)
    : QGraphicsView(parent), game_(game)
{
    setBackgroundBrush(Qt::darkGray);
    setRenderHint(QPainter::Antialiasing);

    //get rid of margin related to fitInView
    setViewportMargins(-2, -2, -2, -2);
    setFrameStyle(QFrame::NoFrame);

    //set min widget size
    setMinimumSize(400, 400);

    //init msg box
    gameEndBox.setText("Checkmate");
    gameEndBox.setInformativeText("Do you want to start a new game?");
    gameEndBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    gameEndBox.setDefaultButton(QMessageBox::Yes);
}

void ChessWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);

    fitInView(ui::kBoardRect, Qt::AspectRatioMode::KeepAspectRatio);
}

void ChessWidget::newGame()
{
    game_ = Chess::Game();
    startTurn();
}

void ChessWidget::startTurn()
{
    Chess::Player active = game_.activePlayer();
    PlayerType ptype = playerType[active];
    std::cout << "Start turn: " << active << (ptype == PlayerType::HUMAN ? "(Human)" : "(AI)") << std::endl;

    if(ptype == PlayerType::HUMAN)
    {
        //HUMAN player -> wait for them to select move
        emit readyToReceiveUserMove();
    }
    else
    {
        //AI player -> wait for move from chess engine
    }
}

void ChessWidget::completeTurn(Chess::Move move)
{
    std::cout << "Selected move: " << move << std::endl;

    try
    {
        game_.performMove(move);
        std::cout << "Performed move: " << move << std::endl;
    }
    catch (...)
    {
        std::cout << "Move failed: " << move << std::endl;
        return;
    }
    emit movePerformed();

    if (game_.isComplete())
    {
        switch(game_.result())
        {
            case Chess::ResultType::Checkmate:
            {
                std::string winner = (game_.activePlayer() == Chess::Player::White ? "Black" : "White");
                gameEndBox.setText(QString::fromStdString("Checkmate - " + winner + " wins"));
                break;
            }
            case Chess::ResultType::Draw:
            case Chess::ResultType::Stalemate:
            default:
                gameEndBox.setText(QString::fromStdString("Stalemate - Nobody wins"));
        }

        if(gameEndBox.exec() == QMessageBox::Yes)
        {
            newGame();
        }
    }
    else
    {
        startTurn();
    }
}

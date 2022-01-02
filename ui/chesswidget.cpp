#include "chesswidget.h"
#include "chess/common.h"

ChessWidget::ChessWidget(QWidget *parent) : QGraphicsView(parent)
{
    setBackgroundBrush(ui::colour::kSceneBackground);
    setRenderHint(QPainter::Antialiasing);

    //get rid of margin related to fitInView
    setViewportMargins(-2, -2, -2, -2);
    setFrameStyle(QFrame::NoFrame);

    //set min widget size
    setMinimumSize(256, 256);

    //init msg box
    gameEndBox.setText("Checkmate");
    gameEndBox.setInformativeText("Do you want to start a new game?");
    gameEndBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    gameEndBox.setDefaultButton(QMessageBox::Yes);
}

void ChessWidget::showGameCompleteDialog(const Chess::ResultType result, const Chess::Player lastPlayer)
{
    switch(result)
    {
        case Chess::ResultType::Checkmate:
        {
            std::string winner = (lastPlayer == Chess::Player::White ? "Black" : "White");
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
        emit requestNewGame();
    }
}

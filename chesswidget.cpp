#include <QFrame>
#include <QMouseEvent>
#include <iostream>
#include "chesswidget.h"

ChessWidget::ChessWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    //init pixmaps
    for(int i=0; i<NUM_PLAYERS; i++) {
        for(int j=0; j<NUM_CHESS_PIECE_TYPES; j++) {
            QString path = ":/images/%1%2.png";
            if(Player(i) == WHITE) {
                piecePixmaps[i][j] = new QPixmap();
                piecePixmaps[i][j]->load(path.arg("w").arg(j));
            } else {
                piecePixmaps[i][j] = new QPixmap;
                piecePixmaps[i][j]->load(path.arg("b").arg(j));
            }
        }
    }

    //init board graphical state
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++) {
        for(int i=0; i<NUM_ROWS; i++) {
            for(int j=0; j<NUM_COLS; j++) {
                boardGraphicalState[pid][i][j] = BGState::NORMAL;
            }
        }
    }

    //init chessboard
    chessBoard = new ChessBoardQGraphicsItem(this);

    //init chesspieces
    for(int i=0; i<NUM_PLAYERS; i++) {
        for(int j=0; j<NUM_CHESS_PIECES; j++) {
            ChessPiece* piece = game.getChessPiece(Player(i), PieceID(j));
            pieces[i][j] = new ChessPieceQGraphicsItem(this, piece);
        }
    }

    //init scene
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
    setBackgroundBrush(Qt::darkGray);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);

    //add chessboard item to scene
    scene->addItem(chessBoard);

    //add chesspiece items to scene
    for(int i=0; i<NUM_PLAYERS; i++) {
        for(int j=0; j<NUM_CHESS_PIECES; j++) {
            scene->addItem(pieces[i][j]);
        }
    }

    //get rid of margin related to fitInView
    setViewportMargins(-2, -2, -2, -2);
    setFrameStyle(QFrame::NoFrame);

    //set min widget size
    setMinimumSize(400, 400);

    //start new game
    newGame(false);
}

ChessWidget::~ChessWidget() {
    delete chessBoard;

    for(int i=0; i<NUM_PLAYERS; i++) {
        for(int j=0; j<NUM_CHESS_PIECE_TYPES; j++) {
            delete pieces[i][j];
        }
    }
}

void ChessWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);

    fitInView(QRect(0,0,BOARD_WIDTH, BOARD_HEIGHT), Qt::AspectRatioMode::KeepAspectRatio);
}

void ChessWidget::mousePressEvent(QMouseEvent* event)
{
    QGraphicsItem* item = itemAt(event->pos());
    if(item && item->type() == CHESSBOARD_QGRAPHICSITEM_TYPE) {
        IBP clickedPos = ChessWidget::getChessboardPosition(event->pos());
        chessBoardItemMousePress(clickedPos);
    }
    QGraphicsView::mousePressEvent(event);
}

void ChessWidget::chessPieceItemMousePress(ChessPiece* piece)
{
    bool cond1 = !isPieceSelected() && getActivePlayer() == piece->getOwner();  //no piece is selected and any owned piece is clicked
    bool cond2 = isPieceSelected() && getSelectedPiece() != piece;              //a piece is currently selected and any other owned piece is clicked

    if(cond1 || cond2) {
        selectPiece(piece);

        //set flag to prevent next mouse release event from deselecting
        isRecentSelection = true;
    }
}

void ChessWidget::chessBoardItemMousePress(IBP pos)
{
    if(isPieceSelected()) {
        PieceID pid = selectedPiece->getId();
        BGState bgs = boardGraphicalState[pid][pos.row][pos.col];

        switch(bgs) {
            case BGState::MOVE:
            case BGState::CAPTURE:
            case BGState::CASTLE:
            case BGState::PROMOTION:
            {
                ChessMove move = getPlayerSelectedMove(selectedPiece, pos);
                if(!move.empty()) {
                    playerTurn(move);
                }
                return;
            }
            case BGState::SOURCE:
            case BGState::NORMAL:
            default:
                deselectPiece();
                return;
        }

    }
}

ChessMove ChessWidget::getPlayerSelectedMove(ChessPiece* piece, IBP moveDst)
{
    ABP moveDstABP = ChessBoard::tranlateIBPoABP(moveDst);
    ChessMoves* moves = game.getChessMoves(piece->getOwner(), piece->getId());

    for(ChessMoves::iterator it = moves->begin(); it != moves->end(); ++it) {
        ChessMove move = *it;
        ABP dst = ChessBoard::getMoveDstABP(move);
        if(dst == moveDstABP) {
            return move;
        }
    }
    return "";
}


void ChessWidget::chessPieceItemMouseRelease(ChessPiece* piece, QPointF point)
{
    IBP clickedPos = ChessWidget::getChessboardPosition(point);
    if(!ChessWidget::isSamePosition(clickedPos, piece->getIBPos())) {
        //if piece dragged to different square -> deselect
        deselectPiece();
    }else if(isRecentSelection) {
        //if mouse press event just selected piece -> ignore mouse release
        isRecentSelection = false;
    } else {
        deselectPiece();
    }
}

IBP ChessWidget::getChessboardPosition(QPointF point)
{
    int x = point.x();
    int y = point.y();

    int i = (y / SQUARE_WIDTH);
    int j = (x / SQUARE_WIDTH);
    return {i,j};
}

bool ChessWidget::isClickInChessBoard(QPointF point)
{
    int x = point.x();
    int y = point.y();
    return (x >= 0 && x<=BOARD_WIDTH) && (y>=0 && y<=BOARD_HEIGHT);
}

bool ChessWidget::isSamePosition(IBP p1, IBP p2)
{
    return p1.col == p2.col && p1.row == p2.row;
}


void ChessWidget::setInitialBoardState() {

}

QPixmap* ChessWidget::getPiecePixmap(PieceType type, Player player) {
    return piecePixmaps[player][type];
}

void ChessWidget::newGame(bool isWAI)
{
    isWhiteAI = isWAI;
    //game.newGame();
    setUnreadyToDisplayMoves();
    deselectPiece()  ;
    startTurn();
}

bool ChessWidget::isReadyToDisplayMoves()
{
    return readyToDisplayMoves;
}

void ChessWidget::setReadyToDisplayMoves()
{
    readyToDisplayMoves = true;
}

void ChessWidget::setUnreadyToDisplayMoves()
{
    readyToDisplayMoves = false;
}

void ChessWidget::selectPiece(ChessPiece* piece)
{
    pieceSelected = true;
    selectedPiece = piece;
    chessBoard->update();
    std::cout << "Selected piece: " << piece->toString() << std::endl;
}

void ChessWidget::deselectPiece()
{
    std::cout << "Deselected piece: " << ((pieceSelected) ? selectedPiece->toString() : "") << std::endl;
    pieceSelected = false;
    chessBoard->update();
}


bool ChessWidget::isPieceSelected()
{
    return pieceSelected;
}


ChessPiece* ChessWidget::getSelectedPiece()
{
    return selectedPiece;
}

BGState ChessWidget::getBGState(int i, int j)
{
    if(pieceSelected) {
        return boardGraphicalState[selectedPiece->getId()][i][j];
    } else {
        return BGState::NORMAL;
    }
}


void ChessWidget::startTurn()
{
    game.generateMoves();
    computeBoardGraphicalStates();
    setReadyToDisplayMoves();
}

void ChessWidget::completeTurn(ChessMove move)
{
    playerTurn(move);
}


void ChessWidget::playerTurn(ChessMove move)
{
    std::cout << "Selected move: " << move << std::endl;
    return;
}

void ChessWidget::AITurn(ChessMove move)
{
    return;
}

bool ChessWidget::performMove(ChessMove move)
{
    return true;
}

void ChessWidget::computeBoardGraphicalStates()
{
    Player activePlayer = game.getActivePlayer();

    //for each chess piece, compute board graphical state
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++) {
        ChessPiece* piece = game.getChessPiece(activePlayer, PieceID(pid));
        ChessMoves* moves = game.getChessMoves(activePlayer, PieceID(pid));

        //initialize board states for that piece to normal
        for(int i=0; i<NUM_ROWS; i++) {
            for(int j=0; j<NUM_COLS; j++) {
                boardGraphicalState[pid][i][j] = BGState::NORMAL;
            }
        }

        //set piece square as source
        IBP pos = piece->getIBPos();
        boardGraphicalState[pid][pos.row][pos.col] = BGState::SOURCE;

        //for each move, set dst square board state
        if(moves == nullptr)
            continue;

        for(ChessMoves::iterator it = moves->begin(); it != moves->end(); ++it) {
            ChessMove move = *it;
            IBP dst = ChessBoard::getMoveDstIBP(move);
            boardGraphicalState[pid][dst.row][dst.col] = BGState::MOVE;
        }
    }
}

Player ChessWidget::getActivePlayer()
{
    return game.getActivePlayer();
}

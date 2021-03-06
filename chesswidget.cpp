#include <QFrame>
#include <QMouseEvent>
#include <QGraphicsItem>
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
                boardGraphicalState[pid][i][j] = SGS::NONE;
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
    newGame();

    //init msg box
    gameEndBox.setText("Checkmate");
    gameEndBox.setInformativeText("Do you want to start a new game?");
    gameEndBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    gameEndBox.setDefaultButton(QMessageBox::Yes);
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
    //no piece is selected and any owned piece is clicked
    bool cond1 = !isPieceSelected() && game.getActivePlayer() == piece->getOwner();

    //a piece is currently selected and any other owned piece is clicked
    bool cond2 = isPieceSelected() && game.getActivePlayer() == piece->getOwner() && getSelectedPiece() != piece;

    //a piece is selected and an enemy piece is clicked -> treat as chessboard click
    bool cond3 = isPieceSelected() && game.getActivePlayer() != piece->getOwner();

    if(cond1 || cond2) {
        selectPiece(piece);

        //set flag to prevent next mouse release event from deselecting
        isRecentSelection = true;
    } else if(cond3) {
        chessBoardItemMousePress(piece->getIBPos());
    }
}

void ChessWidget::chessBoardItemMousePress(IBP pos)
{
    if(isPieceSelected()) {
        PieceID pid = selectedPiece->getId();
        SGS sgs = boardGraphicalState[pid][pos.row][pos.col];

        switch(sgs) {
            case SGS::NORMAL_MOVE:
            case SGS::CAPTURE:
            {
                ChessMove move = getPlayerSelectedMove(selectedPiece, pos);
                if(!move.empty()) {
                    completeTurn(move);
                }
                return;
            }
            case SGS::SOURCE:
            case SGS::NONE:
            default:
                deselectPiece();
                return;
        }
    }
}

ChessMove ChessWidget::getPlayerSelectedMove(ChessPiece* piece, IBP moveDst)
{
    ABP moveDstABP = BoardPosition::tranlateIBPoABP(moveDst);
    ChessMoves* moves = game.getChessMoves(piece->getABPos());

    for(ChessMoves::iterator it = moves->begin(); it != moves->end(); ++it) {
        ChessMove move = *it;
        ABP dst = BoardPosition::getMoveDstABP(move);
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
        //if piece dragged to different square -> treat like chessboard click
        chessBoardItemMousePress(clickedPos);
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

void ChessWidget::newGame()
{
    setUnreadyToDisplayMoves();
    deselectPiece();
    setAllPiecesUnmovable();
    game.setInitialGameState();
    updatePieces();
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

SGS ChessWidget::getBGState(int i, int j)
{
    if(pieceSelected) {
        return boardGraphicalState[selectedPiece->getId()][i][j];
    } else {
        return SGS::NORMAL_MOVE;
    }
}

void ChessWidget::startTurn()
{
    Player active = game.getActivePlayer();
    PlayerType ptype = playerType[active];
    std::cout << "Start turn: " << (active == WHITE ? "White " : "Black ") << (ptype == PlayerType::HUMAN ? "(Human)" : "(AI)") << std::endl;

    if(ptype == PlayerType::HUMAN) {
        //HUMAN player -> wait for them to select move
        computeBoardGraphicalStates();
        setReadyToDisplayMoves();
        setPiecesMovable(active);
    } else {
        //AI player -> wait for move from chess engine
    }
}

void ChessWidget::completeTurn(ChessMove move)
{
    std::cout << "Selected move: " << move << std::endl;

    if(playerType[game.getActivePlayer()] == PlayerType::HUMAN) {
        playerTurn(move);
    } else {
        AITurn(move);
    }

    if(game.performMove(move)) {
        std::cout << "Performed move: " << move << std::endl;
    } else {
        std::cout << "Move failed: " << move << std::endl;
    }

    updatePieces();

    if(game.isCheckmate() || game.isStalemate()) {
        //if game is over
        if((game.isCheckmate())) {
            std::string winner = (game.getActivePlayer() == WHITE ? "Black" : "White");
            gameEndBox.setText(QString::fromStdString("Checkmate - " + winner + " wins"));
        } else if(game.isStalemate()) {
            gameEndBox.setText(QString::fromStdString("Stalemate - Nobody wins"));
        }

        int ret = gameEndBox.exec();
        if(ret == QMessageBox::Yes) {
            newGame();
        } else {

        }
    } else {
        //start next turn
        startTurn();
    }
}

void ChessWidget::playerTurn(ChessMove move)
{
    deselectPiece();
    setAllPiecesUnmovable();
    return;
}

void ChessWidget::AITurn(ChessMove move)
{
    return;
}

void ChessWidget::computeBoardGraphicalStates()
{
    Player activePlayer = game.getActivePlayer();

    //for each chess piece, compute board graphical state
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++) {
        ChessPiece* piece = game.getChessPiece(activePlayer, PieceID(pid));
        ChessMoves* moves = game.getChessMoves(piece->getABPos());

        //initialize board states for that piece to normal
        for(int i=0; i<NUM_ROWS; i++) {
            for(int j=0; j<NUM_COLS; j++) {
                boardGraphicalState[pid][i][j] = SGS::NONE;
            }
        }

        //set piece square as source
        IBP pos = piece->getIBPos();
        boardGraphicalState[pid][pos.row][pos.col] = SGS::SOURCE;

        //for each move, set dst square board state
        if(!moves) {
            continue;
        }

        for(ChessMoves::iterator it = moves->begin(); it != moves->end(); ++it) {
            ChessMove move = *it;
            IBP dst = BoardPosition::getMoveDstIBP(move);
            if(game.isMoveCapture(move)) {
                boardGraphicalState[pid][dst.row][dst.col] = SGS::CAPTURE;
            } else {
                boardGraphicalState[pid][dst.row][dst.col] = SGS::NORMAL_MOVE;
            }
        }
    }

    //clear any previous check
    chessBoard->clearCheck();

    //if active king in check, set king square check state
    if(game.isCheck()) {
        IBP kingPos = game.getKingPos(activePlayer);
        chessBoard->setCheck(kingPos);
    }

}

void ChessWidget::setPiecesMovable(Player player)
{
    Player active = player;
    Player inactive = (player == WHITE ? BLACK : WHITE);

    for(int pid=0; pid<NUM_CHESS_PIECES; pid++) {
        pieces[active][pid]->setFlag(QGraphicsItem::ItemIsMovable, true);
        pieces[inactive][pid]->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
}

void ChessWidget::setAllPiecesUnmovable()
{
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++) {
        pieces[WHITE][pid]->setFlag(QGraphicsItem::ItemIsMovable, false);
        pieces[BLACK][pid]->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
}

void ChessWidget::updatePieces()
{
    for(int i=0; i<NUM_PLAYERS; i++) {
        for(int j=0; j<NUM_CHESS_PIECES; j++) {
            pieces[i][j]->setBoardPosition();
            pieces[i][j]->setPixmap();

            ChessPiece* piece = game.getChessPiece(Player(i), PieceID(j));
            if(piece->isCaptured()) {
                pieces[i][j]->hide();
            } else {
                pieces[i][j]->show();
            }
            pieces[i][j]->update();
        }
    }
}

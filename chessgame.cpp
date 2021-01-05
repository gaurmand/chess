#include <string>
#include <iostream>
#include "constants.h"
#include "chessgame.h"
#include "chesspiece.h"

ChessGame::ChessGame() : ChessBoard()
{
    //init moves array
    for (int i=0; i<NUM_PLAYERS; i++){
        for(int j=0; j<NUM_CHESS_PIECES; j++){
            moves[i][j] = nullptr;
        }
    }

    //init pieces array
    for (int i=0; i<NUM_PLAYERS; i++){
        for(int j=0; j<NUM_CHESS_PIECES; j++){
            pieces[i][j] = new ChessPiece(PieceID(j));
        }
    }

    setInitialGameState();
}

ChessGame::~ChessGame()
{
    for (int i=0; i<NUM_PLAYERS; i++){
        for(int j=0; j<NUM_CHESS_PIECES; j++){
            delete pieces[i][j];
            if(moves[i][j])
                delete moves[i][j];
        }
    }
}


void ChessGame::initChessPiece(PieceID id, Player player, PieceType type, IBP pos)
{
    pieces[player][id]->setPiece(player, type, pos);
    setPiece(pieces[player][id], pos);
}

void ChessGame::clearMoves()
{
    for (int i=0; i<NUM_PLAYERS; i++){
        for(int j=0; j<NUM_CHESS_PIECES; j++){
            if(moves[i][j] != nullptr) {
                delete moves[i][j];
                moves[i][j] = nullptr;
            }
        }
    }
    numAvailableMoves = 0;
}

ChessPiece* ChessGame::getChessPiece(Player player, PieceID id)
{
    return pieces[player][id];
}

ChessMoves* ChessGame::getChessMoves(Player player, PieceID id)
{
    return moves[player][id];
}

bool ChessGame::isValidGameState(IGS state)
{
    return true;
}

bool ChessGame::setGameState(IGS state)
{
    if(!ChessGame::isValidGameState(state)) {
        return false;
    }

    numHalfMoves = state.numHalfMoves;
    numFullMoves = state.numFullMoves;
    active = state.active;
    canShortCastle[0] = state.canShortCastle[0];
    canShortCastle[1] = state.canShortCastle[1];
    canLongCastle[0] = state.canLongCastle[0];
    canLongCastle[1] = state.canLongCastle[1];
    canEnPassant = state.canEnPassant;
    enPassantPosition = state.enPassantPosition;

    //set board array
    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            IBP pos = {i, j};
            ChessPiece* piece = state.board.getPiece(pos);
            setPiece(piece, pos);
            piece->setCaptured(false);  //set pieces on board to not be captured
        }
    }

    return true;
}

IGS ChessGame::getGameState()
{
    return {
        numHalfMoves,
        numFullMoves,
        active,
        {canShortCastle[0], canShortCastle[1]},
        {canLongCastle[0], canLongCastle[1]},
        canEnPassant,
        enPassantPosition,
        *this
    };
}


void ChessGame::setInitialGameState()
{
    //set game state variables to intial state
    numHalfMoves = 0;
    numFullMoves = 0;
    active = WHITE;
    numAvailableMoves = 0;
    _isCheck = false;
    _isCheckmate = false;
    _isStalemate = false;

    //set move state variables to initial state
    for(int i=0; i<NUM_PLAYERS; i++) {
        canShortCastle[i] = true;
        canLongCastle[i] = true;
    }
    canEnPassant = false;
    enPassantPosition = {0, 0};

    //clear moves array and free memory
    clearMoves();

    //clear chess board
    clearBoard();

    //set white pieces to initial positions
    initChessPiece(K, WHITE, KING,      {7, 4});
    initChessPiece(Q, WHITE, QUEEN,     {7, 3});
    initChessPiece(RA, WHITE, ROOK,     {7, 0});
    initChessPiece(RH, WHITE, ROOK,     {7, 7});
    initChessPiece(BC, WHITE, BISHOP,   {7, 2});
    initChessPiece(BF, WHITE, BISHOP,   {7, 5});
    initChessPiece(NB, WHITE, KNIGHT,   {7, 1});
    initChessPiece(NG, WHITE, KNIGHT,   {7, 6});
    initChessPiece(PA, WHITE, PAWN,     {6, 0});
    initChessPiece(PB, WHITE, PAWN,     {6, 1});
    initChessPiece(PC, WHITE, PAWN,     {6, 2});
    initChessPiece(PD, WHITE, PAWN,     {6, 3});
    initChessPiece(PE, WHITE, PAWN,     {6, 4});
    initChessPiece(PF, WHITE, PAWN,     {6, 5});
    initChessPiece(PG, WHITE, PAWN,     {6, 6});
    initChessPiece(PH, WHITE, PAWN,     {6, 7});

    //set black pieces to initial positions
    initChessPiece(K, BLACK, KING,      {0, 4});
    initChessPiece(Q, BLACK, QUEEN,     {0, 3});
    initChessPiece(RA, BLACK, ROOK,     {0, 0});
    initChessPiece(RH, BLACK, ROOK,     {0, 7});
    initChessPiece(BC, BLACK, BISHOP,   {0, 2});
    initChessPiece(BF, BLACK, BISHOP,   {0, 5});
    initChessPiece(NB, BLACK, KNIGHT,   {0, 1});
    initChessPiece(NG, BLACK, KNIGHT,   {0, 6});
    initChessPiece(PA, BLACK, PAWN,     {1, 0});
    initChessPiece(PB, BLACK, PAWN,     {1, 1});
    initChessPiece(PC, BLACK, PAWN,     {1, 2});
    initChessPiece(PD, BLACK, PAWN,     {1, 3});
    initChessPiece(PE, BLACK, PAWN,     {1, 4});
    initChessPiece(PF, BLACK, PAWN,     {1, 5});
    initChessPiece(PG, BLACK, PAWN,     {1, 6});
    initChessPiece(PH, BLACK, PAWN,     {1, 7});

    computeAvailableMoves(WHITE, false);
    printAvailableMoves(WHITE);
}

void ChessGame::computeAvailableMoves(Player player, bool checkCastles) {
    numAvailableMoves = 0;

    for(int j=0; j<NUM_CHESS_PIECES; j++){
        ChessPiece* piece = pieces[player][j];

        if(moves[player][j] != nullptr) {
            delete moves[player][j];
        }

        if(piece->isCaptured()) {
            moves[player][j] = nullptr;
        } else {
            moves[player][j] = getLegalMoves(piece, checkCastles);
        }

        if(moves[player][j] != nullptr) {
            numAvailableMoves += moves[player][j]->size();
        }
    }
}

void ChessGame::printAvailableMoves(Player player)
{
    std::cout << "Print available moves (" << ((player == WHITE) ? "White)" : "Black)") << std::endl;
    for(int j=0; j<NUM_CHESS_PIECES; j++){
        ChessPiece* piece = pieces[player][j];

        std::cout << piece->toString() << ": ";

        if(!piece->isCaptured() && moves[player][j] != nullptr) {
            std::cout << "(" << moves[player][j]->size() << ") " << movesToString(moves[player][j]);
        }

        std::cout << std::endl;
    }
}

std::string ChessGame::movesToString(ChessMoves* moves)
{
    std::string res = "";
    for(ChessMoves::iterator it = moves->begin(); it != moves->end(); ++it) {
        ChessMove move = *it;
        res += move + " ";
    }

    return res;
}

bool ChessGame::performMove(ChessMove move)
{
    //update board state
    ChessBoard::performMove(move);

    //update game state
    numHalfMoves++;
    if(active == BLACK) {
        numFullMoves++;
    }

    switchActivePlayer();
    _isCheck = isPlayerInCheck(active);
    computeAvailableMoves(active, !_isCheck); //enable castles if not in check
    printAvailableMoves(active);

    return true;
}

Player ChessGame::getActivePlayer()
{
    return active;
}

void ChessGame::switchActivePlayer()
{
    if(active == WHITE) {
        active = BLACK;
    } else {
        active = WHITE;
    }
}

void ChessGame::setActivePlayer(Player player)
{
    active = player;
}

ChessMoves* ChessGame::getLegalMoves(ChessPiece* piece, bool checkCastles)
{
    ChessMoves* validMoves = getValidMoves(piece, checkCastles);

    if(!validMoves) {
        return nullptr;
    }

    ChessMoves* legalMoves = new ChessMoves;

    //filter out illegal moves (i.e. moves that put our king in check)
    for(ChessMoves::iterator it = validMoves->begin(); it != validMoves->end(); ++it) {
        ChessMove move = *it;
        if(isMoveLegal(move, false)) {
            legalMoves->push_back(move);
        }
    }

    //Using copy_if to filter
    //std::copy_if (legalMoves->begin(), legalMoves->end(), std::back_inserter(validMoves), &ChessBoard::isValidMove);

    delete validMoves;

    //return legalMoves unlesss it's empty
    if(!legalMoves->empty()) {
        return legalMoves;
    } else {
        delete legalMoves;
        return nullptr;
    }
}

bool ChessGame::isPlayerInCheck(Player player)
{
    Player curr = player;
    Player other = (player == WHITE) ? BLACK : WHITE;
    IBP kingPos = getChessPiece(curr, K)->getIBPos();

    //for each move available to the other player, determine if any are checks (i.e. capture curr player's king)
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++) {
        ChessPiece* piece = getChessPiece(other, PieceID(pid));
        ChessMoves* moves = getValidMoves(piece, false);

        if(piece->isCaptured() || moves == nullptr)
            continue;

        for(ChessMoves::iterator it = moves->begin(); it != moves->end(); ++it) {
            ChessMove move = *it;
            IBP dst = BoardPosition::getMoveDstIBP(move);
            if(dst.row == kingPos.row && dst.col == kingPos.col) {
                return true;
            }
        }

        delete moves;
    }

    return false;
}

bool ChessGame::isValidMoveAvailable()
{
    return numAvailableMoves > 0;
}

bool ChessGame::isCheck()
{
    return _isCheck;
}

bool ChessGame::isCheckmate()
{
    return _isCheckmate;
}

bool ChessGame::isStalemate()
{
    return _isStalemate;
}

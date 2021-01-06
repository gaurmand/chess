#include <string>
#include <sstream>
#include <iostream>
#include "constants.h"
#include "chessgame.h"
#include "chesspiece.h"

ChessGame::ChessGame() : ChessBoard()
{
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
        }
    }

    for(ABPToMovesMap::iterator it = movesMap.begin(); it != movesMap.end(); ++it) {
        ChessMoves* pieceMoves = it->second;
        if(pieceMoves) {
            delete pieceMoves;
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
    for(ABPToMovesMap::iterator it = movesMap.begin(); it != movesMap.end(); ++it) {
        ChessMoves* pieceMoves = it->second;
        if(pieceMoves) {
            delete pieceMoves;
        }
    }

    movesMap.clear();
    numAvailableMoves = 0;
}

ChessPiece* ChessGame::getChessPiece(Player player, PieceID id)
{
    return pieces[player][id];
}

ChessMoves* ChessGame::getChessMoves(ABP pos)
{
    ABPToMovesMap::iterator res = movesMap.find(pos);
    if(res != movesMap.end()) {
        return res->second;
    } else {
        return nullptr;
    }
}

bool ChessGame::isValidGameState(FGS state)
{
    return true;
}

bool ChessGame::setGameState(FGS state)
{
    if(!ChessGame::isValidGameState(state)) {
        return false;
    }

    //split state string into component parts
    std::vector<std::string> compArr;
    std::string temp;
    char delim = ' ';
    std::istringstream ss(state);
    while (std::getline(ss, temp, delim)) {
        if (temp.length() > 0) {
            compArr.push_back(temp);
        }
    }

    //set state variables
    std::string activePlayer = compArr[1];
    active = (activePlayer == "w" ? WHITE : BLACK);
    std::string enPassant = compArr[3];
    canEnPassant = (enPassant != "-");
    if(canEnPassant) {
        enPassantPosition = BoardPosition::tranlateABPoIBP(enPassant);
    } else {
        enPassantPosition = {0, 0};
    }
    numHalfMoves = std::stoi(compArr[4]);
    numFullMoves = std::stoi(compArr[5]);

    //set castling state
    std::string castling = compArr[2];
    canShortCastle[WHITE] = (castling.find("K") != std::string::npos);
    canShortCastle[BLACK] = (castling.find("k") != std::string::npos);
    canLongCastle[WHITE] = (castling.find("Q") != std::string::npos);
    canLongCastle[BLACK] = (castling.find("q") != std::string::npos);

    //set board state
    std::string boardState = compArr[0];
    setBoardState(boardState);
    return true;
}

void ChessGame::setBoardState(std::string FENString)
{
    int i=0, j=0;
    int whitePieceIndex = 0, blackPieceIndex = 0;

    clearBoard();
    for(std::string::iterator it = FENString.begin(); it != FENString.end(); ++it) {
        char curr = *it;
        if (isupper(curr)) {
            //white piece

            ChessPiece* piece = pieces[WHITE][whitePieceIndex++];
            switch(curr) {
                case 'K':
                    piece->setPiece(WHITE, KING, {i, j});
                    break;
                case 'Q':
                    piece->setPiece(WHITE, QUEEN, {i, j});
                    break;
                case 'R':
                    piece->setPiece(WHITE, ROOK, {i, j});
                    break;
                case 'B':
                    piece->setPiece(WHITE, BISHOP, {i, j});
                    break;
                case 'N':
                    piece->setPiece(WHITE, KNIGHT, {i, j});
                    break;
                case 'P':
                    piece->setPiece(WHITE, PAWN, {i, j});
                    break;
            }
            setPiece(piece, {i,j});
            j++; //next col

        } else if(islower(curr)) {
            //black piece

            ChessPiece* piece = pieces[BLACK][blackPieceIndex++];
            switch(curr) {
                case 'k':
                    piece->setPiece(BLACK, KING, {i, j});
                    break;
                case 'q':
                    piece->setPiece(BLACK, QUEEN, {i, j});
                    break;
                case 'r':
                    piece->setPiece(BLACK, ROOK, {i, j});
                    break;
                case 'b':
                    piece->setPiece(BLACK, BISHOP, {i, j});
                    break;
                case 'n':
                    piece->setPiece(BLACK, KNIGHT, {i, j});
                    break;
                case 'p':
                    piece->setPiece(BLACK, PAWN, {i, j});
                    break;
            }
            setPiece(piece, {i,j});
            j++;  //next col

        } else if(isdigit(curr)) {
            //num of blank pieces
            int numBlanks = int(curr) - 48; //char to int conversion
            j += numBlanks;
        } else { //if (curr == '/')
            //row delimiter
            i++;
            j=0;
        }
    }

    //set remaining pieces as captured
    for(int pid = whitePieceIndex; pid < NUM_CHESS_PIECES; pid++) {
        pieces[WHITE][pid]->setCaptured(true);
    }

    for(int pid = blackPieceIndex; pid < NUM_CHESS_PIECES; pid++) {
        pieces[BLACK][pid]->setCaptured(true);
    }
}

std::string ChessGame::toFENString()
{
    std::string boardState = ChessBoard::toFENString();
    std::string activePlayer = (active == WHITE ? "w" : "b");
    std::string enPassant = (canEnPassant ? BoardPosition::tranlateIBPoABP(enPassantPosition) : "-");
    std::string movesState = std::to_string(numHalfMoves) + " " + std::to_string(numFullMoves);

    //get castling state string
    std::string canWhiteShortCastle = (canShortCastle[WHITE] ? "K" : "");
    std::string canWhiteLongCastle = (canLongCastle[WHITE] ? "Q" : "");
    std::string canBlackShortCastle = (canShortCastle[BLACK] ? "k" : "");
    std::string canBlackLongCastle = (canLongCastle[BLACK] ? "q" : "");

    std::string castling = canWhiteShortCastle + canWhiteLongCastle + canBlackShortCastle + canBlackLongCastle;
    castling = (castling.empty() ? "-" : castling);

    //return final state string
    return boardState + " " + activePlayer + " " + castling + " " + enPassant + " " + movesState;
}

void ChessGame::setInitialGameState()
{
    //clear moves array and free memory
    clearMoves();

    //clear chess board
    clearBoard();

    //set board and game state
    setGameState("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");

    numAvailableMoves = 0;
    _isCheck = false;
    _isCheckmate = false;
    _isStalemate = false;

    computeAvailableMoves();
    printAvailableMoves();
}

void ChessGame::computeAvailableMoves() {
    clearMoves();

    for(int j=0; j<NUM_CHESS_PIECES; j++){
        ChessPiece* piece = pieces[active][j];
        std::string pos = piece->getABPos();

        if(!piece->isCaptured()) {
            ChessMoves* moves = getLegalMoves(piece);
            if(moves) {
                movesMap[pos] = moves;
                numAvailableMoves += moves->size();
            }
        }
    }
}

void ChessGame::printAvailableMoves()
{
    std::cout << "Print available moves" << std::endl;

    for(ABPToMovesMap::iterator it = movesMap.begin(); it != movesMap.end(); ++it) {
        std::string pcestr = it->first;
        ChessMoves* pcemoves = it->second;
        std::cout << pcestr << ": ";

        if(pcemoves) {
            std::cout << "(" << pcemoves->size() << ") " << movesToString(pcemoves);
        }
        std::cout << std::endl;
    }
    std::cout << "FEN: " << toFENString() << std::endl;
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
    IBP src = BoardPosition::getMoveSrcIBP(move);
    IBP dst = BoardPosition::getMoveDstIBP(move);

    ChessPiece* srcPiece = getPiece(src);
    Player player = srcPiece->getOwner();

    bool nextTurnEnPassant = false;

    switch(srcPiece->getType()) {
        case KING:
            //if king move -> set castling flags
            canShortCastle[player] = false;
            canLongCastle[player] = false;

            //if move is castle -> move rook
            if(move == "e1g1") {            //white short castle
                movePiece({7,7}, {7,5});
            } else if(move == "e8g8") {     //black short castle
                movePiece({0,7}, {0,5});
            } else if(move == "e1c1") {     //white long castle
                movePiece({7,0}, {7,3});
            } else if(move == "e8c8") {     //white black castle
                movePiece({0,0}, {0,3});
            }
            break;
        case ROOK:
            //if rook move -> set castling flags
            if(srcPiece->getId() == RA) {
                canLongCastle[player] = false;
            } else {
                canShortCastle[player] = false;
            }
            break;
        case PAWN:
        {
            //if double advance -> set en passant flags
            if(player == WHITE && src.row == 6 && dst.row == 4) {
                nextTurnEnPassant = true;
                enPassantPosition = {5, src.col};
            } else if(player == BLACK && src.row == 1 && dst.row == 3) {
                nextTurnEnPassant = true;
                enPassantPosition = {2, src.col};
            }

            //if en passant -> capture pawn above enPassantPosition
            if(canEnPassant && enPassantPosition.row == dst.row && enPassantPosition.col == dst.col) {
                ChessPiece* capturedPawn;
                if(player == WHITE) {
                    //captured pawn is black
                    capturedPawn = getPiece({3, dst.col});
                } else {
                    //capture pawn is white
                    capturedPawn = getPiece({4, dst.col});
                }
                setPiece(nullptr, capturedPawn->getIBPos());
                capturedPawn->setCaptured(true);
            }


            //if promotion -> promote pawn (auto queen for now)
            if (move.size() == 5) {
                srcPiece->setType(QUEEN);
            }
        }
        default:
            break;
    }

    //update board state
    ChessBoard::performMove(move);

    //if pawn double advanced, signal that en passant possible
    //otherwise set to false (can only en passant on immediate turn right after enemy double advance)
    canEnPassant = nextTurnEnPassant;

    //update game state
    numHalfMoves++;
    if(active == BLACK) {
        numFullMoves++;
    }

    switchActivePlayer();
    _isCheck = isPlayerInCheck(active);
    computeAvailableMoves(); //enable castles if not in check
    _isCheckmate = _isCheck && numAvailableMoves == 0;
    _isStalemate = !_isCheck && numAvailableMoves == 0;
    printAvailableMoves();

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

ChessMoves* ChessGame::getLegalMoves(ChessPiece* piece)
{
    ChessMoveTypeOpt opts = {
        !_isCheck && canShortCastle[active],
        !_isCheck && canLongCastle[active],
        true,
        canEnPassant,
        enPassantPosition
    };

    ChessMoves* validMoves = getValidMoves(piece, opts);

    if(!validMoves) {
        return nullptr;
    }

    ChessMoves* legalMoves = new ChessMoves;

    //filter out illegal moves (i.e. moves that put our king in check)
    for(ChessMoves::iterator it = validMoves->begin(); it != validMoves->end(); ++it) {
        ChessMove move = *it;

        if(isMoveLegal(move)) {
            legalMoves->push_back(move);
        }
    }

    //Using copy_if to filter
    //std::copy_if (legalMoves->begin(), legalMoves->end(), std::back_inserter(validMoves), &ChessBoard::isValidMove);

    delete validMoves;

    //return legalMoves unless it's empty
    if(!legalMoves->empty()) {
        return legalMoves;
    } else {
        delete legalMoves;
        return nullptr;
    }
}

bool ChessGame::isMoveLegal(ChessMove move)
{
    FGS prevState = toFENString(); //save game state
    ChessBoard::performMove(move);
    bool res = !isPlayerInCheck(active);
    setGameState(prevState); //restore prev game state
    return res;
}

bool ChessGame::isPlayerInCheck(Player player)
{
    Player curr = player;
    Player other = (player == WHITE) ? BLACK : WHITE;
    IBP kingPos = getKingPos(curr);

    //for each move available to the other player, determine if any are checks (i.e. capture curr player's king)
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++) {
        ChessPiece* piece = getChessPiece(other, PieceID(pid));

        ChessMoveTypeOpt opts = {
            false,
            false,
            true,   //only check promotions since they can cause checks
            false,
            {0,0}
        };

        ChessMoves* moves = getValidMoves(piece, opts);

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

IBP ChessGame::getKingPos(Player player)
{
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++) {
        if(pieces[player][pid]->getType() == KING) {
            return pieces[player][pid]->getIBPos();
        }
    }

    return {-1,-1}; //should throw exception
}

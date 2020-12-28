#include <algorithm>
#include <map>
#include "chessboard.h"
#include "chesspiece.h"

int rowToRank[8] = {8, 7, 6, 5, 4, 3, 2, 1};
char colToFile[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
std::map<int, int> rankToRow = {{1, 7}, {2, 6}, {3, 5}, {4, 4}, {5, 3}, {6, 2}, {7, 1}, {8, 0}};
std::map<char, int> fileToCol = {{'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7}};

ChessBoard::ChessBoard()
{
    clearBoard();
}

void ChessBoard::clearBoard()
{
    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            board[i][j] = nullptr;
        }
    }
}


ChessPiece* ChessBoard::movePiece(IBP src, IBP dst)
{
    ChessPiece* dstPiece = getPiece(dst);
    ChessPiece* srcPiece = getPiece(src);

    if(srcPiece == nullptr)
        return nullptr;

    setPiece(srcPiece, dst);
    return dstPiece;
}

ChessPiece* ChessBoard::getPiece(IBP src)
{
    return board[src.row][src.col];
}

void ChessBoard::setPiece(ChessPiece* piece, IBP dst)
{
    if(piece) {
        piece->setIBPos(dst);
    }
    board[dst.row][dst.col] = piece;
}

ChessMoves* ChessBoard::getValidMoves(ChessPiece* piece)
{
    ChessMoves* legalMoves = getLegalMoves(piece);
    if(legalMoves == nullptr) {
        return nullptr;
    }
    ChessMoves* validMoves = new ChessMoves;

    for(ChessMoves::iterator it = legalMoves->begin(); it != legalMoves->end(); ++it) {
        ChessMove move = *it;
        if(isValidMove(move)) {
            validMoves->push_back(move);
        }
    }

//    //Using copy_if
//    std::copy_if (legalMoves->begin(), legalMoves->end(), std::back_inserter(validMoves), &ChessBoard::isValidMove);

    if(legalMoves != nullptr) {
        delete legalMoves;
    }

    if(validMoves->size() > 0) {
        return validMoves;
    } else {
        return nullptr;
    }
}

ChessMoves* ChessBoard::getLegalMoves(ChessPiece* piece)
{
    PieceType type = piece->getType();
    switch(type) {
        case KING:
        case QUEEN:
        case ROOK:
        case BISHOP:
        case KNIGHT:
            return nullptr;
        case PAWN:
            return getPawnMoves(piece);
    }
}

ChessMoves* ChessBoard::getPawnMoves(ChessPiece* piece)
{
    ChessMoves* moves = new ChessMoves;
    Player player = piece->getOwner();
    IBP pos = piece->getIBPos();

    if(player == WHITE){
        IBP top = {pos.row-1, pos.col};
        IBP topLeft = {pos.row-1, pos.col-1};
        IBP topRight = {pos.row-1, pos.col+1};

        //ignore if in row 0
        if(pos.row == 0) {
            delete moves;
            return nullptr;
        }

        //if no piece in top pos -> push move
        ChessPiece* topPiece = getPiece(top);
        if(!topPiece) {
            moves->push_back(createMove(pos, top));
        }

        //if pawn not in leftmost column, check top left capture
        if(pos.col-1 >= MIN_COL_INDEX) {
            //if enemy piece in top left pos -> push capture
            ChessPiece* topLeftPiece = getPiece(topLeft);
            if(topLeftPiece && topLeftPiece->getOwner() != player) {
                moves->push_back(createMove(pos, topLeft));
            }
        }

        //if pawn not in rightmost column, check top right capture
        if(pos.col+1 <= MAX_COL_INDEX) {
            //if enemy piece in top right pos -> push capture
            ChessPiece* topRightPiece = getPiece(topRight);
            if(topRightPiece && topRightPiece->getOwner() != player) {
                moves->push_back(createMove(pos, topRight));
            }
        }
    } else {
        IBP bottom = {pos.row+1, pos.col};
        IBP bottomLeft = {pos.row+1, pos.col-1};
        IBP bottomRight = {pos.row+1, pos.col+1};

        //ignore if in row 7
        if(pos.row == 7) {
            delete moves;
            return nullptr;
        }

        //if no piece in bottom pos -> push move
        ChessPiece* bottomPiece = getPiece(bottom);
        if(!bottomPiece) {
            moves->push_back(createMove(pos, bottom));
        }

        //if pawn not in leftmost column, check bottom left capture
        if(pos.col-1 >= MIN_COL_INDEX) {
            //if enemy piece in bottom left pos -> push capture
            ChessPiece* bottomLeftPiece = getPiece(bottomLeft);
            if(bottomLeftPiece && bottomLeftPiece->getOwner() != player) {
                moves->push_back(createMove(pos, bottomLeft));
            }
        }

        //if pawn not in rightmost column, check bottom right capture
        if(pos.col+1 <= MAX_COL_INDEX) {
            //if enemy piece in bottom right pos -> push capture
            ChessPiece* bottomRightPiece = getPiece(bottomRight);
            if(bottomRightPiece && bottomRightPiece->getOwner() != player) {
                moves->push_back(createMove(pos, bottomRight));
            }
        }
    }


    if(moves->size() > 0) {
        return moves;
    } else {
        delete moves;
        return nullptr;
    }
}

bool ChessBoard::isValidMove(ChessMove move)
{
    return true;
}

ChessMove ChessBoard::createMove(IBP src, IBP dst, bool isPromotion, PieceType promotiontType)
{
    ChessMove res = "";
    ABP srcABP = ChessBoard::tranlateIBPoABP(src);
    ABP dstABP = ChessBoard::tranlateIBPoABP(dst);
    res = srcABP + dstABP;

    return res;
}

IBP ChessBoard::tranlateABPoIBP(ABP pos)
{
    int rank = std::stoi(pos.substr(1,1));
    char file = pos[0];
    return {rankToRow[rank],fileToCol[file]};
}

ABP ChessBoard::tranlateIBPoABP(IBP pos)
{
    int rank = rowToRank[pos.row];
    char file = colToFile[pos.col];
    return file+std::to_string(rank);
}

IBP ChessBoard::getMoveSrc(ChessMove move)
{
    ABP src = move.substr(0,2);
    return ChessBoard::tranlateABPoIBP(src);
}

IBP ChessBoard::getMoveDst(ChessMove move)
{
    ABP dst = move.substr(2,2);
    return ChessBoard::tranlateABPoIBP(dst);
}


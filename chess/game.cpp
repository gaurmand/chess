#include "game.h"

#include <algorithm>
#include <cassert>
#include <cmath>


namespace Chess
{

namespace InitialPosition
{
    std::array<BP, NUM_PLAYERS> King = {BP{7, 4}, BP{0, 4}};
    std::array<BP, NUM_PLAYERS> ShortRook = {BP{7, 7}, BP{0, 7}};
    std::array<BP, NUM_PLAYERS> LongRook = {BP{0, 7}, BP{0, 0}};
}

namespace IP = InitialPosition;

void Game::setFENState(const FENState& state)
{
    Position::setFENState(state);
    update();
}

void Game::performMove(const Move& move, bool checkLegal)
{
    performMove(move.src(), move.dst(), checkLegal);
}

void Game::performMove(const BP& src, const BP& dst, bool checkLegal)
{
    // If the game is complete, then there are no legal moves to make
    if (result_ != ResultType::Incomplete)
    {
        throw 1;
    }

    if (checkLegal && !isLegalMove(src, dst))
    {
        throw 1;
    }

    MoveType type = moveType(src.toANBP(), dst.toANBP());
    executeMove(src, dst, type);
    switchActivePlayer();

    // Update full move clock
    if (active_ == Player::White)
    {
        numFullMoves_++;
    }

    // Update half move clock
    if (pieceAt(dst)->type() == PieceType::Pawn || type == MoveType::Capture)
    {
        numHalfMoves_ = 0;
    }
    else
    {
        numHalfMoves_++;
    }

    update();
}

void Game::update()
{
    isInCheck_ = isCheck();
    computeLegalMoves();

    // update result
    if (numLegalMoves_ == 0)
    {
        result_ = isInCheck_ ? ResultType::Checkmate : ResultType::Stalemate;
    }
    else
    {
        result_ = ResultType::Incomplete;
    }
}


void Game::executeMove(const BP& src, const BP& dst, MoveType type)
{
    assert(src.isValid());
    assert(dst.isValid());
    bool canEnPassant = false;

    Piece* srcPiece = pieceAt(src);
    const Piece* dstPiece = pieceAt(dst);
    assert(srcPiece != nullptr);
    assert(srcPiece->owner() == active_);
    assert(dstPiece == nullptr || dstPiece->owner() != active_);

    // Perform special move actions
    switch(type)
    {
        case MoveType::Normal:
        {
            // if pawn double advance, check if this enables en-passant
            const bool isDoubleAdvance = srcPiece->type() == PieceType::Pawn && std::abs(dst.row() - src.row()) == 2;
            if (isDoubleAdvance)
            {
                assert(dstPiece == nullptr);
                assert(active_ == Player::White ? dst.row() == 4 : dst.row() == 3);
                assert(active_ == Player::White ? src.row() == 6 : src.row() == 1);
                canEnPassant = executeDoubleAdvance(dst);
            }
        }
        case MoveType::Capture:
        {
            // if rook or king move, disable castling
            const bool isKingOrRookMove = srcPiece->type() == PieceType::King || srcPiece->type() == PieceType::Rook;
            if (isKingOrRookMove)
            {
                executeKingOrRookMove(src, srcPiece);
            }
            break;
        }
        case MoveType::EnPassant:
            assert(canEnPassant_);
            assert(dst == enPassantPosition_);
            assert(dstPiece == nullptr);
            assert(srcPiece->type() == PieceType::Pawn);
            assert(active_ == Player::White ? dst.row() == 2 : dst.row() == 5);
            assert(active_ == Player::White ? src.row() == 3 : src.row() == 4);
            executeEnPassant();  // capture en passant piece
            break;
        case MoveType::Promotion:
            assert(srcPiece->type() == PieceType::Pawn);
            assert(active_ == Player::White ? dst.row() == 0 : dst.row() == 7);
            assert(active_ == Player::White ? src.row() == 1 : src.row() == 6);
            executePromotion(srcPiece);  // promote piece type to queen
            assert(srcPiece->type() == PieceType::Queen);
            break;
        case MoveType::LongCastle:
            assert(srcPiece->type() == PieceType::King);
            if (active_ == Player::White)
            {
                assert(src == BP("e1"));
                assert(dst == BP("c1"));
                assert(pieceAt(BP("d1")) == nullptr);
                assert(pieceAt(BP("c1")) == nullptr);
                assert(pieceAt(BP("b1")) == nullptr);
                const Piece* rook = pieceAt(BP("a1"));
                assert(rook != nullptr);
                assert(rook->type() == PieceType::Rook);
                assert(rook->owner() == Player::White);
            }
            else
            {
                assert(src == BP("e8"));
                assert(dst == BP("c8"));
                assert(pieceAt(BP("d8")) == nullptr);
                assert(pieceAt(BP("c8")) == nullptr);
                assert(pieceAt(BP("b8")) == nullptr);
                const Piece* rook = pieceAt(BP("a8"));
                assert(rook != nullptr);
                assert(rook->type() == PieceType::Rook);
                assert(rook->owner() == Player::Black);
            }
            executeLongCastle();  // move rook
            break;
        case MoveType::ShortCastle:
            assert(srcPiece->type() == PieceType::King);
            if (active_ == Player::White)
            {
                assert(src == BP("e1"));
                assert(dst == BP("g1"));
                assert(pieceAt(BP("f1")) == nullptr);
                assert(pieceAt(BP("g1")) == nullptr);
                const Piece* rook = pieceAt(BP("h1"));
                assert(rook != nullptr);
                assert(rook->type() == PieceType::Rook);
                assert(rook->owner() == Player::White);
            }
            else
            {
                assert(src == BP("e8"));
                assert(dst == BP("g8"));
                assert(pieceAt(BP("f8")) == nullptr);
                assert(pieceAt(BP("g8")) == nullptr);
                const Piece* rook = pieceAt(BP("h8"));
                assert(rook != nullptr);
                assert(rook->type() == PieceType::Rook);
                assert(rook->owner() == Player::Black);
            }
            executeShortCastle();  // move rook
            break;
        case MoveType::Unknown:
        default:
            throw 1;
    }
    canEnPassant_ = canEnPassant;

    Board::performMove(src, dst);
}

bool Game::executeDoubleAdvance(const BP& dst)
{
    const BP leftPos = dst.left();
    const BP rightPos = dst.right();
    const Piece* leftPiece = leftPos.isValid() ? pieceAt(leftPos) : nullptr;
    const Piece* rightPiece = rightPos.isValid() ? pieceAt(rightPos) : nullptr;
    const bool isLeftEnemyPawn = leftPiece != nullptr && leftPiece->type() == PieceType::Pawn && leftPiece->owner() != active_;
    const bool isRightEnemyPawn = rightPiece != nullptr && rightPiece->type() == PieceType::Pawn && rightPiece->owner() != active_;

    if (isLeftEnemyPawn || isRightEnemyPawn)
    {
        const BP enPassantPos = active_ == Player::White ? dst.bottom() : dst.top();
        enPassantPosition_ = enPassantPos;
        return true;
    }
    return false;
}

void Game::executeKingOrRookMove(const BP& src, const Piece* srcPiece)
{
    const bool isKingMove = srcPiece->type() == PieceType::King;
    const bool isShortRookMove = srcPiece->type() == PieceType::Rook && src == IP::ShortRook[active_];
    const bool isLongRookMove = srcPiece->type() == PieceType::Rook && src == IP::LongRook[active_];

    if (canShortCastle_[active_] && (isKingMove || isShortRookMove))
    {
        canShortCastle_[active_] = false;
    }
    else if (canLongCastle_[active_] && (isKingMove || isLongRookMove))
    {
        canLongCastle_[active_] = false;
    }
}

void Game::executeEnPassant()
{
    const BP capturePos = active_ == Player::White ? enPassantPosition_.bottom() : enPassantPosition_.top();
    assert(capturePos.isValid());

    Piece* capturePiece = pieceAt(capturePos);
    assert(capturePiece != nullptr);
    assert(capturePiece->type() == PieceType::Pawn);
    assert(capturePiece->owner() != active_);

    // capture pawn in en-passant position
    capturePiece->capture();
    setPiece(capturePos, nullptr);
}

void Game::executePromotion(Piece* srcPiece)
{
    srcPiece->setType(PieceType::Queen);
}

void Game::executeLongCastle()
{
    if (active_ == Player::White)
    {
        Board::performMove(BP(7,0), BP(7,3));   // a1 d1
    }
    else
    {
        Board::performMove(BP(0,0), BP(0,3));   // a8 d8
    }
    canLongCastle_[active_] = false;
}

void Game::executeShortCastle()
{
    if (active_ == Player::White)
    {
        Board::performMove(BP(7,7), BP(7,5));   // h1 f1
    }
    else
    {
        Board::performMove(BP(0,7), BP(0,5));   // h8 f8
    }
    canShortCastle_[active_] = false;
}

Move Game::move(const BP& src, const BP& dst) const
{
    if (!src.isValid() || !dst.isValid())
    {
        return Move();
    }

    const Move* move = findMove(src.toANBP(), dst.toANBP());
    if (move)
    {
        return *move;
    }
    return Move();
}

bool Game::isLegalMove(const BP& src, const BP& dst) const
{
    return findMove(src.toANBP(), dst.toANBP()) != nullptr;
}

MoveType Game::moveType(const BP& src, const BP& dst) const
{
    const Move* move = findMove(src.toANBP(), dst.toANBP());
    if (move)
    {
        return move->type();
    }
    return MoveType::Unknown;
}

const Move* Game::findMove(const ANBP& src, const ANBP& dst) const
{
    auto movesRes = legalMoves_.find(src);
    if (movesRes == legalMoves_.end())
    {
        return nullptr;
    }

    const std::vector<Move>& moves = movesRes->second;
    auto moveRes = std::find(moves.begin(), moves.end(), Move(src, dst));

    if (moveRes != moves.end())
    {
        return &(*moveRes);
    }

    return nullptr;
}

std::vector<Move> Game::moves() const
{
    std::vector<Move> allMoves;
    for (const auto& movePair: legalMoves_)
    {
        const std::vector<Move>& moves = movePair.second;
        allMoves.insert(allMoves.end(), moves.begin(), moves.end());
    }
    return allMoves;
}

std::vector<Move> Game::moves(const BP& pos) const
{
    auto movesRes = legalMoves_.find(pos.toANBP());
    if (movesRes != legalMoves_.end())
    {
        return movesRes->second;
    }
    return std::vector<Move>();
}

void Game::computeCandidateMoves(std::map<ANBP, std::vector<Move>>& map)
{
    map.clear();
    for (Piece& piece: pieces_[active_])
    {
        if (piece.isCaptured())
        {
            continue;
        }

        assert(piece.isValid());
        assert(piece.pos().isValid());
        assert(piece.owner() == active_);

        const ANBP pos = piece.pos().toANBP();
        switch(piece.type())
        {
            case PieceType::King:
                computeKingMoves(piece, map[pos]);
                break;
            case PieceType::Queen:
                computeQueenMoves(piece, map[pos]);
                break;
            case PieceType::Rook:
                computeRookMoves(piece, map[pos]);
                break;
            case PieceType::Bishop:
                computeBishopMoves(piece, map[pos]);
                break;
            case PieceType::Knight:
                computeKnightMoves(piece, map[pos]);
                break;
            case PieceType::Pawn:
                computePawnMoves(piece, map[pos]);
                break;
            default:
                throw 1;
        }
    }
}

void Game::computeLegalMoves()
{
    auto checkIllegalMove = [&](const Move& move)
    {
        // TODO: for castles we need to check if intermediate positions under attack
        const auto saveState = save();
        executeMove(move.src(), move.dst(), move.type());
        const bool res = isCheck();
        restore(saveState);
        return res;
    };

    numLegalMoves_ = 0;
    computeCandidateMoves(legalMoves_);
    for (auto& movePair: legalMoves_)
    {
        std::vector<Move>& posMoves = movePair.second;
        posMoves.erase(
                    std::remove_if(posMoves.begin(), posMoves.end(), checkIllegalMove),
                    posMoves.end());
        numLegalMoves_ += static_cast<int>(posMoves.size());
    }
}

void Game::computeKingMoves(Piece& piece, std::vector<Move>& moves)
{
    const BP pos = piece.pos();

    pushValidMove(pos, pos.top(), moves);          // push top move
    pushValidMove(pos, pos.topRight(), moves);     // push topRight move
    pushValidMove(pos, pos.right(), moves);        // push right move
    pushValidMove(pos, pos.bottomRight(), moves);  // push bottomRight move
    pushValidMove(pos, pos.bottom(), moves);       // push bottom move
    pushValidMove(pos, pos.bottomLeft(), moves);   // push bottomLeft move
    pushValidMove(pos, pos.left(), moves);         // push left move
    pushValidMove(pos, pos.topLeft(), moves);      // push topLeft move

    // check castles if not in check
    if (isInCheck())
        return;

    if (canShortCastle_[active_])
    {
        if (active_ == Player::White)
        {
//            const Piece* scPiece = board_[7][7]; // h1
//            assert(scPiece != nullptr);
//            assert(scPiece->type_ == PieceType::Rook);
//            assert(scPiece->owner_ == active_);
//            assert(pos == initWhiteKingPos);

            if (board_[7][6] == nullptr && board_[7][5] == nullptr) // f1 & g1
            {
                moves.emplace_back(pos, BP(7, 6), MoveType::ShortCastle); // g1
            }
        }
        else
        {
//            const Piece* scPiece = board_[0][7]; // h8
//            assert(scPiece != nullptr);
//            if (scPiece->type_ != PieceType::Rook)
//            {
//                throw 1;
//            }
//            assert(scPiece->type_ == PieceType::Rook);
//            assert(scPiece->owner_ == active_);
//            assert(pos == initBlackKingPos);

            if (board_[0][6] == nullptr && board_[0][5] == nullptr) // f8 & g8
            {
                moves.emplace_back(pos, BP(0, 6), MoveType::ShortCastle); // g8
            }
        }
    }

    if (canLongCastle_[active_])
    {
        if (active_ == Player::White)
        {
//            const Piece* lcPiece = board_[7][0]; // a1
//            assert(lcPiece != nullptr);
//            assert(lcPiece->type_ == PieceType::Rook);
//            assert(lcPiece->owner_ == active_);
//            assert(pos == initWhiteKingPos);

            if (board_[7][3] == nullptr && board_[7][2] == nullptr && board_[7][1] == nullptr) // d1, c1, & b1
            {
                moves.emplace_back(pos, BP(7, 2), MoveType::LongCastle); // c1
            }
        }
        else
        {
//            const Piece* lcPiece = board_[0][0]; // a8
//            assert(lcPiece != nullptr);
//            assert(lcPiece->type_ == PieceType::Rook);
//            assert(lcPiece->owner_ == active_);
//            assert(pos == initBlackKingPos);

            if (board_[0][3] == nullptr && board_[0][2] == nullptr && board_[0][1] == nullptr) // d8, c8, & b8
            {
                moves.emplace_back(pos, BP(0, 2), MoveType::LongCastle); // c8
            }
        }
    }
}

void Game::computeKnightMoves(Piece& piece, std::vector<Move>& moves)
{
    const BP pos = piece.pos();

    pushValidMove(pos, BP(pos.row()-2, pos.col()+1), moves);  // push top-right hook move
    pushValidMove(pos, BP(pos.row()-1, pos.col()+2), moves);  // push right-top hook move
    pushValidMove(pos, BP(pos.row()+1, pos.col()+2), moves);  // push right-bottom hook move
    pushValidMove(pos, BP(pos.row()+2, pos.col()+1), moves);  // push bottom-right hook move
    pushValidMove(pos, BP(pos.row()+2, pos.col()-1), moves);  // push bottom-left hook move
    pushValidMove(pos, BP(pos.row()+1, pos.col()-2), moves);  // push left-bottom hook move
    pushValidMove(pos, BP(pos.row()-1, pos.col()-2), moves);  // push left-top hook move
    pushValidMove(pos, BP(pos.row()-2, pos.col()-1), moves);  // push top-left hook move
}

void Game::computePawnMoves(Piece& piece, std::vector<Move>& moves)
{
    // TODO: set promotion flag?

    const BP pos = piece.pos();
    if(active_ == Player::White){

        const bool topMove = pushValidMove(pos, pos.top(), moves, true, false); //push top move

        //if pawn in starting row (6) and top square is empty, push double advance move
        if(topMove && pos.row() == 6) {
            pushValidMove(pos, BP(pos.row()-2, pos.col()), moves, true, false);
        }

        const BP topLeft = pos.topLeft();
        const BP topRight = pos.topRight();

        pushValidMove(pos, topLeft, moves, false); // push top left capture
        pushValidMove(pos, topRight, moves, false); // push top right capture

        // check en passant
        if (canEnPassant_)
        {
            if (enPassantPosition_ == topLeft)
            {
                moves.emplace_back(pos, topLeft, MoveType::EnPassant);
            }
            else if (enPassantPosition_ == topRight)
            {
                moves.emplace_back(pos, topRight, MoveType::EnPassant);
            }
        }

    } else {

        const bool bottomMove = pushValidMove(pos, pos.bottom(), moves, true, false); //push bottom move

        //if pawn in starting row (1) and bottom square is empty, push double advance move
        if(bottomMove && pos.row() == 1) {
            pushValidMove(pos, BP(pos.row()+2, pos.col()), moves, true, false);
        }

        const BP botLeft = pos.bottomLeft();
        const BP botRight = pos.bottomRight();

        pushValidMove(pos, botLeft, moves, false); // push bottom left capture
        pushValidMove(pos, botRight, moves, false); // push bottom right capture

        // check en passant
        if (canEnPassant_)
        {
            if (enPassantPosition_ == botLeft)
            {
                moves.emplace_back(pos, botLeft, MoveType::EnPassant);
            }
            else if (enPassantPosition_ == botRight)
            {
                moves.emplace_back(pos, botRight, MoveType::EnPassant);
            }
        }
    }
}

void Game::computeQueenMoves(Piece& piece, std::vector<Move>& moves)
{
    pushMoveHelper(piece.pos(), moves, true, true);
}

void Game::computeRookMoves(Piece& piece, std::vector<Move>& moves)
{
    pushMoveHelper(piece.pos(), moves, true, false);
}

void Game::computeBishopMoves(Piece& piece, std::vector<Move>& moves)
{
    pushMoveHelper(piece.pos(), moves, false, true);
}

void Game::pushMoveHelper(const BP& src, std::vector<Move>& moves, bool checkStraights, bool checkDiagonals)
{
    assert(checkStraights || checkDiagonals);

    auto searchMoves = [&](const BP& src, const BP& dst)
    {
        const Piece* dstPiece = pieceAt(dst);
        if(dstPiece != nullptr)
        {
            if(dstPiece->owner() != active_)
            {
                // push capture
                moves.emplace_back(src, dst, MoveType::Capture);
            }
            return true;  //end search
        }
        else
        {
            // push normal move
            moves.emplace_back(src, dst, MoveType::Normal);
            return false;  // continue search
        }
    };

    if (checkStraights)
    {
        //check up col
        for(int i = src.row()-1; i>=MIN_ROW; i--){
            if(searchMoves(src, BP(i, src.col())))
                break;
        }

        //check right row
        for(int j = src.col()+1; j<=MAX_COL; j++){
            if(searchMoves(src, BP(src.row(), j)))
                break;
        }

        //check down col
        for(int i = src.row()+1; i<=MAX_ROW; i++){
            if(searchMoves(src, BP(i, src.col())))
                break;
        }

        //check left row
        for(int j = src.col()-1; j>=MIN_COL; j--){
            if(searchMoves(src, BP(src.row(), j)))
                break;
        }
    }

    if (checkDiagonals)
    {
        //check top right diagonal
        for(int i = src.row()-1, j = src.col()+1; i>=MIN_ROW && j<=MAX_COL; i--, j++){
            if(searchMoves(src, BP(i, j)))
                break;
        }

        //check bottom right diagonal
        for(int i = src.row()+1, j = src.col()+1; i<=MAX_ROW && j<=MAX_COL; i++, j++){
            if(searchMoves(src, BP(i, j)))
                break;
        }

        //check bottom left diagonal
        for(int i = src.row()+1, j = src.col()-1; i<=MAX_ROW && j>=MIN_COL; i++, j--){
            if(searchMoves(src, BP(i, j)))
                break;
        }

        //check top left diagonal
        for(int i = src.row()-1, j = src.col()-1; i>=MIN_ROW && j>=MIN_COL; i--, j--){
            if(searchMoves(src, BP(i, j)))
                break;
        }
    }
}

bool Game::pushValidMove(const BP& src, const BP& dst, std::vector<Move>& moves, bool allowNormalMoves, bool allowCaptures)
{
    assert(allowNormalMoves || allowCaptures);

    if(!dst.isValid()) {
        return false;
    }

    const bool isPromotion = [&]()
    {
        const Piece* srcPiece = pieceAt(src);
        if (srcPiece->type() != PieceType::Pawn)
            return false;
        if (srcPiece->owner() == Player::White && dst.row() == 0)
            return true;
        if (srcPiece->owner() == Player::Black && dst.row() == 7)
            return true;
        return false;
    }();

    const Piece* dstPiece = pieceAt(dst);
    if (allowNormalMoves && dstPiece == nullptr)
    {
        // push normal move
        moves.emplace_back(src, dst, isPromotion ? MoveType::Promotion : MoveType::Normal);
        return true;
    }
    else if (allowCaptures && dstPiece != nullptr && dstPiece->owner() != active_)
    {
        // push capture
        moves.emplace_back(src, dst, isPromotion ? MoveType::Promotion : MoveType::Capture);
        return true;
    }
    return false;
}

bool Game::isCheck()
{
    switchActivePlayer();
    computeCandidateMoves(checkMoves_);
    switchActivePlayer();
    for (const auto& movePair: checkMoves_)
    {
        const std::vector<Move>& moves = movePair.second;
        auto res = std::find_if(moves.begin(),
                                moves.end(),
                                [&](const Move& move) {return move.dst() == kingPosition_[active_];});
        if (res != moves.end())
        {
            return true;
        }
    }
    return false;
}

}; // namespace Chess

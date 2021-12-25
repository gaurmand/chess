#ifndef BOARDPOS_H
#define BOARDPOS_H

#include <string>

namespace Chess {

typedef std::string AlgebraicNotationBoardPosition;
typedef AlgebraicNotationBoardPosition ANBP;

class BoardPosition
{
public:
    BoardPosition() {}
    BoardPosition(int row, int col): row_(row), col_(col) {}
    BoardPosition(const ANBP& pos);
    ANBP toANBP() const;

    int row() const { return row_; }
    int col() const { return col_; }
    bool isValid() const;

    BoardPosition top() const { return BoardPosition(row_-1, col_);}
    BoardPosition topRight() const { return BoardPosition(row_-1, col_+1);}
    BoardPosition right() const { return BoardPosition(row_, col_+1);}
    BoardPosition bottomRight() const { return BoardPosition(row_+1, col_+1);}
    BoardPosition bottom() const { return BoardPosition(row_+1, col_);}
    BoardPosition bottomLeft() const { return BoardPosition(row_+1, col_-1);}
    BoardPosition left() const { return BoardPosition(row_, col_-1);}
    BoardPosition topLeft() const { return BoardPosition(row_-1, col_-1);}

    friend bool operator==(const BoardPosition& lhs, const BoardPosition& rhs);
    friend bool operator!=(const BoardPosition& lhs, const BoardPosition& rhs);

private:
    int row_ = -1;
    int col_ = -1;
};

typedef BoardPosition BP;

std::ostream& operator<<(std::ostream&, const BP&);


};  // namespace Chess

#endif // BOARDPOS_H

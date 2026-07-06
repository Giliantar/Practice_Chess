#include "Bishop.h"
#include <cmath>

Bishop::Bishop(Color color) : Figure(color, Type::Bishop) {}

bool Bishop::canMove(int fromRow, int fromCol, int toRow, int toCol,
                     const std::vector<std::vector<Figure*>>& board) const
{
    if (fromRow == toRow && fromCol == toCol) return false;

    int rowDiff = std::abs(toRow - fromRow);
    int colDiff = std::abs(toCol - fromCol);

    // Слон ходит по диагонали
    if (rowDiff != colDiff) return false;

    int rowStep = (fromRow < toRow) ? 1 : -1;
    int colStep = (fromCol < toCol) ? 1 : -1;

    int r = fromRow + rowStep;
    int c = fromCol + colStep;
    while (r != toRow && c != toCol) {
        if (board[r][c] != nullptr) return false;
        r += rowStep;
        c += colStep;
    }

    return (board[toRow][toCol] == nullptr ||
            board[toRow][toCol]->getColor() != color);
}
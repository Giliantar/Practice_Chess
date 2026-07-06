#include "Knight.h"
#include <cmath>

Knight::Knight(Color color) : Figure(color, Type::Knight) {}

bool Knight::canMove(int fromRow, int fromCol, int toRow, int toCol,
                     const std::vector<std::vector<Figure*>>& board) const
{
    int rowDiff = std::abs(toRow - fromRow);
    int colDiff = std::abs(toCol - fromCol);

    // Конь ходит буквой "Г": 2+1 или 1+2
    if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)) {
        return (board[toRow][toCol] == nullptr ||
                board[toRow][toCol]->getColor() != color);
    }
    return false;
}
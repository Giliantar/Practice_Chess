#include "King.h"
#include <cmath>

King::King(Color color) : Figure(color, Type::King) {}

bool King::canMove(int fromRow, int fromCol, int toRow, int toCol,
                   const std::vector<std::vector<Figure*>>& board) const
{
    int rowDiff = std::abs(toRow - fromRow);
    int colDiff = std::abs(toCol - fromCol);

    // Король ходит на 1 клетку в любом направлении
    if (rowDiff <= 1 && colDiff <= 1 && (rowDiff + colDiff > 0)) {
        return (board[toRow][toCol] == nullptr ||
                board[toRow][toCol]->getColor() != color);
    }
    return false;
}
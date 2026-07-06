#include "Rook.h"

Rook::Rook(Color color) : Figure(color, Type::Rook) {}

bool Rook::canMove(int fromRow, int fromCol, int toRow, int toCol,
                   const std::vector<std::vector<Figure*>>& board) const
{
    if (fromRow == toRow && fromCol == toCol) return false;

    // Ход по вертикали
    if (fromCol == toCol) {
        int step = (fromRow < toRow) ? 1 : -1;
        for (int r = fromRow + step; r != toRow; r += step) {
            if (board[r][fromCol] != nullptr) return false;
        }
        return (board[toRow][toCol] == nullptr ||
                board[toRow][toCol]->getColor() != color);
    }

    // Ход по горизонтали
    if (fromRow == toRow) {
        int step = (fromCol < toCol) ? 1 : -1;
        for (int c = fromCol + step; c != toCol; c += step) {
            if (board[fromRow][c] != nullptr) return false;
        }
        return (board[toRow][toCol] == nullptr ||
                board[toRow][toCol]->getColor() != color);
    }

    return false;
}
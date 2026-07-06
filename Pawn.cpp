#include "Pawn.h"
#include <cmath>

Pawn::Pawn(Color color) : Figure(color, Type::Pawn) {}

bool Pawn::canMove(int fromRow, int fromCol,
                   int toRow, int toCol,
                   const std::vector<std::vector<Figure*>>& board) const
{
    int direction = (color == Color::White) ? -1 : 1;
    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;

    // Ход на 1 клетку вперёд
    if (colDiff == 0 && rowDiff == direction) {
        if (board[toRow][toCol] == nullptr) return true;
    }

    // Ход на 2 клетки с начальной позиции
    int startRow = (color == Color::White) ? 6 : 1;
    if (colDiff == 0 && rowDiff == 2 * direction && fromRow == startRow) {
        if (board[toRow][toCol] == nullptr &&
            board[fromRow + direction][fromCol] == nullptr) {
            return true;
        }
    }

    // Взятие по диагонали
    if (std::abs(colDiff) == 1 && rowDiff == direction) {
        if (board[toRow][toCol] != nullptr &&
            board[toRow][toCol]->getColor() != color) {
            return true;
        }
    }

    return false;
}
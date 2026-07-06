#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"

Queen::Queen(Color color) : Figure(color, Type::Queen) {}

bool Queen::canMove(int fromRow, int fromCol, int toRow, int toCol,
                    const std::vector<std::vector<Figure*>>& board) const
{
    Rook rook(color);
    Bishop bishop(color);

    return rook.canMove(fromRow, fromCol, toRow, toCol, board) ||
           bishop.canMove(fromRow, fromCol, toRow, toCol, board);
}
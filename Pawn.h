#ifndef PAWN_H
#define PAWN_H

#include "Figure.h"

class Pawn : public Figure
{
public:
    Pawn(Color color);

    bool canMove(int fromRow, int fromCol,
                 int toRow, int toCol,
                 const std::vector<std::vector<Figure*>>& board) const override;
};

#endif // PAWN_H
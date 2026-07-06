#ifndef KNIGHT_H
#define KNIGHT_H

#include "Figure.h"

class Knight : public Figure {
public:
    Knight(Color color);

    bool canMove(int fromRow, int fromCol, int toRow, int toCol,
                 const std::vector<std::vector<Figure*>>& board) const override;
};

#endif
#ifndef ROOK_H
#define ROOK_H

#include "Figure.h"

class Rook : public Figure {
public:
    Rook(Color color);

    bool canMove(int fromRow, int fromCol, int toRow, int toCol,
                 const std::vector<std::vector<Figure*>>& board) const override;
};

#endif
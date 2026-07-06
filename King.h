#ifndef KING_H
#define KING_H

#include "Figure.h"

class King : public Figure {
public:
    King(Color color);

    bool canMove(int fromRow, int fromCol, int toRow, int toCol,
                 const std::vector<std::vector<Figure*>>& board) const override;
};

#endif
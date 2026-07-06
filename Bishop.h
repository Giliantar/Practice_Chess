#ifndef BISHOP_H
#define BISHOP_H

#include "Figure.h"

class Bishop : public Figure {
public:
    Bishop(Color color);

    bool canMove(int fromRow, int fromCol, int toRow, int toCol,
                 const std::vector<std::vector<Figure*>>& board) const override;
};

#endif
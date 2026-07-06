#ifndef QUEEN_H
#define QUEEN_H

#include "Figure.h"

class Queen : public Figure {
public:
    Queen(Color color);

    bool canMove(int fromRow, int fromCol, int toRow, int toCol,
                 const std::vector<std::vector<Figure*>>& board) const override;
};

#endif
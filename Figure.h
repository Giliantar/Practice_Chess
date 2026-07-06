#ifndef FIGURE_H
#define FIGURE_H
#include <arm_fix.h>
#include <QString>
#include <vector>   // ← добавляем для board

class Figure
{
public:
    enum class Color { White, Black };
    enum class Type { Pawn, Rook, Knight, Bishop, Queen, King };

    Figure(Color color, Type type);
    virtual ~Figure() = default;

    Color getColor() const;
    Type getType() const;
    QString getSymbol() const;

    virtual bool canMove(int fromRow, int fromCol,
                         int toRow, int toCol,
                         const std::vector<std::vector<Figure*>>& board) const = 0;
    QString getName() const;

protected:
    Color color;
    Type type;
};

#endif // FIGURE_H
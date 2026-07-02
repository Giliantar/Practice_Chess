#ifndef FIGURE_H
#define FIGURE_H
#include <arm_fix.h>
#include <QString>

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

protected:
    Color color;
    Type type;
};

#endif // FIGURE_H
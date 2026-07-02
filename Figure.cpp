#include "Figure.h"

Figure::Figure(Color color, Type type)
    : color(color), type(type) {}

Figure::Color Figure::getColor() const {
    return color;
}
Figure::Type Figure::getType() const {
    return type;
}

QString Figure::getSymbol() const {
    switch (type) {
    case Type::Pawn:   return color == Color::White ? "♙" : "♟";
    case Type::Rook:   return color == Color::White ? "♖" : "♜";
    case Type::Knight: return color == Color::White ? "♘" : "♞";
    case Type::Bishop: return color == Color::White ? "♗" : "♝";
    case Type::Queen:  return color == Color::White ? "♕" : "♛";
    case Type::King:   return color == Color::White ? "♔" : "♚";
    }
    return "?";
}
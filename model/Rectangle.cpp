//
// Created by denis on 06.11.23.
//

#include "Rectangle.h"

Rectangle::Rectangle(const std::pair<Coord, Coord> &pos) : _leftTop(pos.first), _rightBottom(pos.second) {

}

void Rectangle::Save(std::ostream &str) const {
    str << _leftTop << _rightBottom;
}

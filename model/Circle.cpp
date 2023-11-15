//
// Created by denis on 06.11.23.
//

#include "Circle.h"

Circle::Circle(const std::pair<Coord, int> &pos) : _center(pos.first), _radius(pos.second) {

}

void Circle::Save(std::ostream &str) const {
    str << _center << _radius;
}

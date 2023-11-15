//
// Created by denis on 06.11.23.
//

#include "Point.h"

void Point::Save(std::ostream &str) const {
    str << _pos;
}

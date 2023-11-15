//
// Created by denis on 06.11.23.
//

#include "Line.h"

Line::Line(const std::pair<Coord, Coord> &pos) : _start(pos.first), _end(pos.second) {

}

void Line::Save(std::ostream &str) const {
    str << _start << _end;
}

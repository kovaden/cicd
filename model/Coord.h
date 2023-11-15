//
// Created by denis on 06.11.23.
//

#ifndef GRAPH_EDIT_COORD_H
#define GRAPH_EDIT_COORD_H

#include <ostream>

class Coord {
public:
    int x;
    int y;

    bool operator == (const Coord &other) const {
        return x == other.x && y == other.y;
    }
};

inline std::ostream &operator <<(std::ostream &os, const Coord &c) {
    os << c.x << ":" << c.y;
    return os;
}

#endif //GRAPH_EDIT_COORD_H

//
// Created by denis on 06.11.23.
//

#ifndef PRINT_IP_POINT_H
#define PRINT_IP_POINT_H

#include "Primitive.h"
#include "Coord.h"

class Point : public Primitive {
public:
    Point(const Coord &where) : _pos(where) {}

    virtual ~Point() {}
    void Save(std::ostream &str) const override;
    [[nodiscard]] bool is_at(const Coord &pos) const override {
        return _pos == pos;
    }

    friend class View;
private:
    Coord _pos;
};


#endif //PRINT_IP_POINT_H

//
// Created by denis on 06.11.23.
//

#ifndef PRINT_IP_RECTANGLE_H
#define PRINT_IP_RECTANGLE_H

#include "Primitive.h"
#include "Coord.h"
#include "../error.h"

class Rectangle : public Primitive {
public:
    explicit Rectangle(const std::pair<Coord, Coord> &pos);

    void Save(std::ostream &str) const override;

    ~Rectangle() override = default;

    [[nodiscard]] bool is_at(const Coord &) const override {
        // TODO - calculate if the point is lying in the rectangle
        throw NotImplemened();
    }

    friend class View;
private:
    Coord _leftTop;
    Coord _rightBottom;
};


#endif //PRINT_IP_RECTANGLE_H

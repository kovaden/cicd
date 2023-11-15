//
// Created by denis on 06.11.23.
//

#ifndef PRINT_IP_CIRCLE_H
#define PRINT_IP_CIRCLE_H

#include "Primitive.h"
#include "Coord.h"
#include "../error.h"

#include <memory>

class Circle : public Primitive {
public:
    Circle(const std::pair<Coord, int> &pos);

    void Save(std::ostream &str) const override;

    ~Circle() override = default;

    [[nodiscard]] bool is_at(const Coord &) const override {
        // TODO - calculate if the point is lying in the circle
        throw NotImplemened();
    }

    friend class View;
private:
    Coord _center;
    int _radius;
};


#endif //PRINT_IP_CIRCLE_H

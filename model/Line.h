//
// Created by denis on 06.11.23.
//

#ifndef PRINT_IP_LINE_H
#define PRINT_IP_LINE_H

#include "Primitive.h"
#include "Coord.h"
#include "../error.h"

#include <tuple>

class Line : public Primitive {
public:
    Line(const std::pair<Coord, Coord> &pos);

    void Save(std::ostream &str) const override;

    virtual ~Line() {};

    [[nodiscard]] bool is_at(const Coord &) const override {
        // TODO - calculate if the point is lying on the line
        throw NotImplemened();
    }

    friend class View;
private:
    Coord _start;
    Coord _end;
};


#endif //PRINT_IP_LINE_H

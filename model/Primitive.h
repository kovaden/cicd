//
// Created by denis on 06.11.23.
//

#ifndef PRINT_IP_PRIMITIVE_H
#define PRINT_IP_PRIMITIVE_H

#include <ostream>
#include "Coord.h"
/*
 *   Abstract base class for graphic primitives
 */
class Primitive {
public:
    virtual ~Primitive() = default;

    virtual void Save(std::ostream &str) const = 0;

    [[nodiscard]] virtual bool is_at(const Coord &pos) const = 0;
};

#endif //PRINT_IP_PRIMITIVE_H

//
// Created by denis on 15.11.23.
//

#ifndef GRAPH_EDIT_MODEL_H
#define GRAPH_EDIT_MODEL_H

#include "Circle.h"
#include "Line.h"
#include "Point.h"
#include "Rectangle.h"

#include <variant>
#include <memory>

using primitive_ptr = std::variant<
        std::shared_ptr<Circle>,
        std::shared_ptr<Line>,
        std::shared_ptr<Point>,
        std::shared_ptr<Rectangle>
        >;

#endif //GRAPH_EDIT_MODEL_H

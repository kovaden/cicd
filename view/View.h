//
// Created by denis on 13.11.23.
//

#ifndef GRAPH_EDIT_VIEW_H
#define GRAPH_EDIT_VIEW_H

#include "../model/Circle.h"
#include "../model/Line.h"
#include "../model/Point.h"
#include "../model/Rectangle.h"
#include "../model/Primitive.h"

#include <memory>

class View {
public:
    void drawPrimitive(const std::shared_ptr<Circle>& circle);
    void drawPrimitive(const std::shared_ptr<Line>& line);
    void drawPrimitive(const std::shared_ptr<Point>& point);
    void drawPrimitive(const std::shared_ptr<Rectangle>& rectangle);
};


#endif //GRAPH_EDIT_VIEW_H

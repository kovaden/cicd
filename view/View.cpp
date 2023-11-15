//
// Created by denis on 13.11.23.
//

#include "View.h"
#include <iostream>

using namespace std;

void View::drawPrimitive(const std::shared_ptr<Circle>& circle) {
    cout << "Circle (" << circle->_center << ":" << circle->_radius << ")" << endl;
}

void View::drawPrimitive(const std::shared_ptr<Line>& line) {
    cout << "Line (" << line->_start << " - " << line->_end << ")" << endl;
}

void View::drawPrimitive(const std::shared_ptr<Point>& point) {
    cout << "Point (" << point->_pos << ")" << endl;
}

void View::drawPrimitive(const std::shared_ptr<Rectangle>& rectangle) {
    cout << "Rectangle (" << rectangle->_leftTop << " -> " << rectangle->_rightBottom << ")" << endl;
}

//
// Created by denis on 06.11.23.
//

#include "Controller.h"
#include "../model/Point.h"
#include "../model/Line.h"
#include "../model/Rectangle.h"
#include "../model/Circle.h"

#include "../view/View.h"

#include <fstream>
#include <utility>

using namespace std;

std::shared_ptr<Document> Controller::create_document() {
    current_document = make_shared<Document>();
    return current_document;
}

void Controller::Export(const std::string &filename) {
    ofstream ofs(filename, std::ios::binary);
    ofs << *current_document;
}

void Controller::Import(const std::string &filename) {
    ifstream ifs(filename);
    ifs >> *current_document;
}

primitive_ptr Controller::CreatePrimitive(Controller::PrimitiveType what, CreateParams param)
{
    primitive_ptr ret;

    switch (what) {
        case pPoint:
            ret = make_shared<Point>(std::get<Coord>(param));
            break;
        case pLine:
            ret = make_shared<Line>(std::get<std::pair<Coord, Coord>>(param));
            break;
        case pRectangle:
            ret = make_shared<Rectangle>(std::get<std::pair<Coord, Coord>>(param));
            break;
        case pCircle:
            ret = make_shared<Circle>(std::get<std::pair<Coord, int>>(param));
            break;
        default:
            throw invalid_argument("unknown primitive");
    }
    current_document->AddPrimitive(ret);
    return ret;
}

std::vector<primitive_ptr> Controller::FindPrimitives(const Coord &coord) {
    return current_document->FindPrimitives(coord);
}

void Controller::DeletePrimitive(primitive_ptr p) {
    current_document->DeletePrimitive(std::move(p));
}

struct DrawVisitor {
    void operator ()(std::shared_ptr<Circle> c) {v.drawPrimitive(std::move(c));};
    void operator ()(std::shared_ptr<Line> c) {v.drawPrimitive(std::move(c));};
    void operator ()(std::shared_ptr<Point> c) {v.drawPrimitive(std::move(c));};
    void operator ()(std::shared_ptr<Rectangle> c) {v.drawPrimitive(std::move(c));};
    View &v;
};

void Controller::Draw() {
    DrawVisitor dv{view};

    for (auto primitive : current_document->primitives) {

        std::visit(dv, primitive);
    }
}


//
// Created by denis on 06.11.23.
//

#ifndef GRAPH_EDIT_CONTROLLER_H
#define GRAPH_EDIT_CONTROLLER_H

#include "../model/Primitive.h"
#include "../Document.h"
#include "../model/Coord.h"

#include <memory>
#include <string>
#include <variant>
#include "../view/View.h"

class Controller {
public:
    std::shared_ptr<Document> create_document();

    void Import(const std::string &filename);
    void Export(const std::string &filename);

    enum PrimitiveType{pPoint, pLine, pRectangle, pCircle};

    using CreateParams = std::variant<Coord, std::pair<Coord, Coord>, std::pair<Coord, int>>;

    primitive_ptr CreatePrimitive(PrimitiveType what, CreateParams params);

    std::vector<primitive_ptr> FindPrimitives(const Coord &coord);
    void DeletePrimitive(primitive_ptr primitive);

    void Draw();
private:
    std::shared_ptr<Document> current_document;
    View view;
};


#endif //GRAPH_EDIT_CONTROLLER_H

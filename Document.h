//
// Created by denis on 05.11.23.
//

#ifndef PRINT_IP_DOCUMENT_H
#define PRINT_IP_DOCUMENT_H

#include "model/Model.h"
#include "model/Coord.h"
#include "error.h"

#include <vector>
#include <memory>
#include <fstream>

class Document {
public:
private:

    std::vector<primitive_ptr> primitives;

    void Export(std::ostream &str) const;
    void Import(std::istream &str);

    void AddPrimitive(const primitive_ptr pptr);
    std::vector<primitive_ptr> FindPrimitives(const Coord &coord);
    void DeletePrimitive(primitive_ptr sharedPtr);

    friend class Controller;
    friend std::ostream &operator <<(std::ostream &str, const Document &doc);
    friend std::istream &operator>>(std::istream &str, Document &doc);
};

#endif //PRINT_IP_DOCUMENT_H

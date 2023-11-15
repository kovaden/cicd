//
// Created by denis on 05.11.23.
//

#include "Document.h"

#include <algorithm>
#include <utility>

using namespace std;

std::ostream &operator<<(std::ostream &str, const Document &doc){
    doc.Export(str);
    return str;
}

std::istream &operator>>(std::istream &str, Document &doc){
    doc.Import(str);
    return str;
}

void Document::Export(std::ostream &str) const {
    for (const auto &item : primitives) {

        // All options in primitive_ptr are inherited from Primitive
        // Perhaps would be better to make uniform conversion but no idea how yet
        std::shared_ptr<Primitive> p = std::get<0>(item);

        p->Save(str);
    }
}

void Document::Import(std::istream &str) {
    primitives.clear();

    if (!str) {
        return;
    }
    // TODO: Load stream, parse it and create primitives

}

void Document::AddPrimitive(const primitive_ptr p) {
    primitives.emplace_back(p);
}

std::vector<primitive_ptr> Document::FindPrimitives(const Coord &coord) {
    std::vector<primitive_ptr> ret;
    copy_if(primitives.begin(), primitives.end(), ret.begin(),
            [coord](auto &p){
                std::shared_ptr<Primitive> pp = std::get<0>(p);
                return pp->is_at(coord);
                });

    return ret;
}

void Document::DeletePrimitive(primitive_ptr delp) {
    auto it = find_if(primitives.begin(), primitives.end(),
                      [delp](auto &p) {
                          return p == delp;
                            });
    if (it != primitives.end()) {
        primitives.erase(it);
    } else {
        throw PrimitiveNotFound();
    }
}



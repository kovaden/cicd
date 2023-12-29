//
// Created by denis on 22.12.23.
//

#ifndef HELLOWORLD_HASHFACTORY_H
#define HELLOWORLD_HASHFACTORY_H

#include <functional>
#include <string>
#include "FileHash.h"

typedef std::function<FileHash::HashBlockType (const std::vector<char > &)> hash_fn_type;

class HashFactory {
public:
    static FileHash::hash_fn_type get_hash_fn(const std::string &fn_name);
};


#endif //HELLOWORLD_HASHFACTORY_H

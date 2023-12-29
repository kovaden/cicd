//
// Created by denis on 22.12.23.
//

#ifndef HELLOWORLD_FILEHASH_H
#define HELLOWORLD_FILEHASH_H

#include <vector>
#include <functional>
#include <cstddef>
#include <cstdint>
#include <string>
#include <variant>
#include "IFile.h"

class FileHash {
public:
    using HashBlockType = std::variant<uint32_t, std::string>;
    typedef std::function<FileHash::HashBlockType (const std::vector<char > &)> hash_fn_type;

    FileHash(IFile &owner);

    // (true,hash) if success, (false,<undefined>) if not
    std::pair<bool, HashBlockType> block_hash(int block_no);
    int max_block() {
        return _owner.max_block();
    }
private:
    IFile &_owner;
    std::vector<HashBlockType> hash_list;
    bool has_completed{false};

    hash_fn_type hash_calc_fn{nullptr};
};


#endif //HELLOWORLD_FILEHASH_H

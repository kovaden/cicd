//
// Created by denis on 29.12.23.
//

#ifndef HELLOWORLD_PROGRAMOPTIONS_H
#define HELLOWORLD_PROGRAMOPTIONS_H

#include <string>

class ProgramOptions {
public:
    static int block_size() {
        return _instance._block_size;
    }
    static void block_size(int val) {
        _instance._block_size = val;
    }

    static std::string &hash() {
        return _instance._hash_name;
    };
    static void hash(const std::string &hash) {
        _instance._hash_name = hash;
    }

private:
    int _block_size{64};
    std::string _hash_name{"CRC32"};
    static ProgramOptions _instance;
};


#endif //HELLOWORLD_PROGRAMOPTIONS_H

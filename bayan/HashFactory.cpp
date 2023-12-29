//
// Created by denis on 22.12.23.
//

#include "HashFactory.h"
#include "md5hash.h"

#include <stdexcept>

using namespace std;

static FileHash::HashBlockType crc_hash(const vector<char> &inp)
{
    uint32_t crc=0xFFFFFFFF;

    for (auto ch : inp) {
        for(size_t j=0; j<8; j++) {
            uint32_t b= (ch ^ crc) & 1;
            crc >>= 1;
            if (b)
                crc = crc ^ 0xEDB88320;
            ch >>= 1;
        }
    }

    return ~crc;
}

static FileHash::HashBlockType md5_hash(const vector<char> &buf)
{
    return GetMD5String(buf);
}

FileHash::hash_fn_type HashFactory::get_hash_fn(const string &fn_name)
{
    if (fn_name == "CRC32") {
        return crc_hash;
    }  else if (fn_name == "MD5") {
        return md5_hash;
    }

    throw invalid_argument(fn_name + " is unknown hash");
}

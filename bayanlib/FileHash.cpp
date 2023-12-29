//
// Created by denis on 22.12.23.
//

#include "FileHash.h"
#include "HashFactory.h"
#include "ProgramOptions.h"
#include "cassert"

using namespace std;

FileHash::FileHash(IFile &owner) : _owner(owner)
{
    string hash_name = ProgramOptions::hash();
    hash_calc_fn = HashFactory::get_hash_fn(hash_name);
}

std::pair<bool, FileHash::HashBlockType > FileHash::block_hash(int block_no) {
    int read_blocks = hash_list.size();

    if (block_no < read_blocks) {
        return {true, hash_list[block_no]};
    }

    if (has_completed) {
        // File has finished but block past EOF requested
        return {false, {}};
    }

    // Block are expected to be read sequentially
    assert(block_no == read_blocks);

    auto res = _owner.read(block_no);

    switch (res.first) {
        case IFile::FRS_OK:
        case IFile::FRS_DONE:
        {
            assert(hash_calc_fn != nullptr);
            auto hash = hash_calc_fn(res.second);
            hash_list.push_back(hash);
            has_completed = res.first == IFile::FRS_DONE;
            return {true, hash};
        }
        case IFile::FRS_EOF:
            has_completed = true;
            return {false, {}};
        case IFile::FRS_ERR:
        default:
            return {false, {}};
    }
}


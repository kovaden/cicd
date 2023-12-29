//
// Created by denis on 22.12.23.
//

#ifndef HELLOWORLD_BAYANFILE_H
#define HELLOWORLD_BAYANFILE_H

#include <vector>
#include <string>
#include <iterator>

#include <boost/filesystem.hpp>

#include "FileHash.h"
#include "FileOpener.h"
#include "IFile.h"

static const int INVALID_BLOCK = -1;

class invalid_block : public std::invalid_argument
{
public:
    invalid_block() : invalid_argument("block is invalid") {};
};

/*
 *   Representation of file for Bayan file compare
 */
class BayanFile : public IFile {
public:

    explicit BayanFile(const boost::filesystem::path &p);

    BayanFile(BayanFile &) = delete;
    BayanFile(const BayanFile &) = delete;

    const FileHash &hash() {
        if (!hash_calculated) {
            calculate_hash();
        }
        return hash_list;
    }

    class hash_iterator {
    public:
        hash_iterator(FileHash &container, int block=INVALID_BLOCK) : _block(block), _owner(container) {}

        typedef typename std::forward_iterator_tag iterator_category;
        typedef typename FileHash::HashBlockType value_type;
        typedef uint32_t difference_type;
        typedef typename FileHash::HashBlockType *     pointer;
        typedef typename FileHash::HashBlockType &    reference;

        bool operator != (const hash_iterator &other) const {
            return _block != other._block;
        }

        void operator ++() {
            ++_block;
            if (_block >= _owner.max_block()) {
                _block = INVALID_BLOCK;
            }
        }

        FileHash::HashBlockType operator *() {
            if (_block == INVALID_BLOCK || _block > _owner.max_block()) {
                throw invalid_block();
            }

            auto res = _owner.block_hash(_block);
            assert(res.first);

            return res.second;
        }
    private:
        int _block{INVALID_BLOCK};
        FileHash &_owner;
    };

    hash_iterator begin() {
        return {hash_list, 0};
    }

    hash_iterator end() {
        return {hash_list};
    }

    off_type size() const {
        return _size;
    }

    off_type bytes_read() const {
        return _bytes_read;
    }

    bool equal(BayanFile &other) {
        if (size() != other.size()) {
            return false;
        }

        // Make sure both files are kept opened during compare to not reopen them on every read
        FileOpener fo1(*this);
        FileOpener fo2(other);

        return std::equal(begin(), end(), other.begin());
    }

    std::pair<FileReadStatus, std::vector<char> &> read(int block_no) override;
    void open() override;
    void close() override;
    int max_block() override;
    bool opened() {
        return _fin.is_open();
    }

    const std::string &name() const {
        return _fname;
    }
private:
    std::string _fname;
    off_type _size{0};
    std::streamsize _block_size{1024};

    FileHash hash_list;

    bool hash_calculated{false};
    void calculate_hash();

    std::vector<char> _buffer;
    std::fstream _fin;
    int _bytes_read{0};
};


#endif //HELLOWORLD_BAYANFILE_H

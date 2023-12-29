//
// Created by denis on 22.12.23.
//

#include <string>
#include <iostream>
#include <istream>
#include <cassert>
#include "BayanFile.h"
#include "boost/filesystem.hpp"
#include "ProgramOptions.h"

namespace fs = boost::filesystem;
using namespace std;

BayanFile::BayanFile(const boost::filesystem::path &p) : _fname(p.string()), hash_list(*this)
{
    _size = fs::file_size(p.string());
    _block_size = ProgramOptions::block_size();

}

std::pair<IFile::FileReadStatus, std::vector<char> &>  BayanFile::read(int block_no)
{
    if (! _fin.is_open()) {
        open();
    }
    if (_fin.bad()) {
        cout << "Error in file " << _fname << " Eof: " << _fin.eof() << " Fail: " << _fin.fail() <<
            " Bad: " << _fin.bad() << endl;
        return {FRS_ERR, _buffer};
    }

//    cout << "File " << _fname << " reading block " << block_no << endl;

    off_type pos = _block_size * block_no;
    if (_fin.eof() || pos > size() ) {
        // End of file reached
        cout << "File " << _fname << " EOF "<< endl;
        return {FRS_EOF, _buffer};
    }
    _fin.seekg(pos, std::fstream::beg);
    _fin.read(_buffer.data(), _block_size);

    if (_fin.bad() ) {
        cout << "Error in file " << _fname << " Eof: " << _fin.eof() << " Fail: " << _fin.fail() <<
             " Bad: " << _fin.bad() << " " << std::strerror(errno) << endl;
        return {FRS_ERR, _buffer};
    }

    auto bytes_read = _fin.gcount();

    _bytes_read += bytes_read;

    if (bytes_read < _block_size) {
        memset(_buffer.data() + bytes_read, 0, _block_size - bytes_read);
        return {FRS_DONE, _buffer};
    }

    return {FRS_OK, _buffer};
}

void BayanFile::open() {
    _fin.open(_fname, fstream::in | fstream::binary);
    assert(_fin.good());
    _buffer.resize(_block_size, 0);

//    std::cout << "Opened " << _fname << endl;
}

void BayanFile::close() {
    _fin.close();
    _buffer.resize(0);
//    std::cout << "Closed " << _fname << endl;
}

int BayanFile::max_block() {
    int extra = size() % _block_size ? 1 : 0;
    return extra + size() / _block_size;
}


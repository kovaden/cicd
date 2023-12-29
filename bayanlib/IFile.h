//
// Created by denis on 28.12.23.
//

#ifndef HELLOWORLD_IFILE_H
#define HELLOWORLD_IFILE_H

#include <vector>
#include <fstream>

/* Interface IFile defines basic operations over a file */
class IFile {
public:
    enum FileReadStatus {FRS_OK, FRS_DONE, FRS_EOF, FRS_ERR};

    typedef std::fstream::off_type off_type;

    virtual std::pair<FileReadStatus, std::vector<char> &> read(int block_no) = 0;
    virtual void open() = 0;
    virtual void close() = 0;
    virtual int max_block() = 0;

    virtual ~IFile() = default;
};

#endif //HELLOWORLD_IFILE_H

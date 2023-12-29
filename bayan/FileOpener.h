//
// Created by denis on 28.12.23.
//

#ifndef HELLOWORLD_FILEOPENER_H
#define HELLOWORLD_FILEOPENER_H

#include "IFile.h"
#include <memory>

class FileOpener {
public:
    FileOpener(IFile &file) : _file(file) {
        _file.open();
    }

    virtual ~FileOpener() {
        _file.close();
    }
private:
    IFile &_file;
};


#endif //HELLOWORLD_FILEOPENER_H

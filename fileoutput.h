//
// Created by denis on 19.12.23.
//

#ifndef BULK_FILEOUTPUT_H
#define BULK_FILEOUTPUT_H

#include <fstream>
#include "icommand.h"
#include "output.h"

class FileHandler : public IOutHandler
{
public:
    void log_batch(const ICommandBatch &batch, bool is_batch_end) override;
    void log_command(const Command &cmd) override;

    virtual ~FileHandler() {
        if (the_file.is_open()) {
            the_file.close();
        }
    }
private:
    std::ofstream the_file;
};


#endif //BULK_FILEOUTPUT_H

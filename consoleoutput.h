//
// Created by denis on 19.12.23.
//

#ifndef BULK_CONSOLEOUTPUT_H
#define BULK_CONSOLEOUTPUT_H

#include "output.h"
#include "icommand.h"
#include "command.h"
#include <iostream>

class ConsoleHandler : public IOutHandler
{
public:
    ConsoleHandler() = default;

    void log_batch(const ICommandBatch &, bool is_batch_end) override {
        if (is_batch_end) {
            std::cout << std::endl;
        } else {
            std::cout << "bulk: ";
        }
    }

    void log_command(const Command &cmd) override {
        std::cout << cmd.str() << " ";
    }

    virtual ~ConsoleHandler() {}

private:
};


#endif //BULK_CONSOLEOUTPUT_H

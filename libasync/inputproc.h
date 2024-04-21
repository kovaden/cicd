//
// Created by denis on 19.12.23.
//

#ifndef BULK_INPUTPROC_H
#define BULK_INPUTPROC_H

#include <string>
#include <memory>
#include "icommand.h"

class CommandProcessor
{
public:
    CommandProcessor(int N, ICommandBatchCreator &batch_creator, ICommandExecutor &executor) :
        block_size(N),
        _batch_creator(batch_creator),
        _executor(executor)
    {
        current_block = std::unique_ptr<ICommandBatch>(_batch_creator.create());
    }

    ~CommandProcessor() {
        close();
    }

    void process(const std::string &cmd_str);

    void close() {
        if (level == 0) {
            send_block();
        }
    }
private:
    int level{0};
    size_t block_size;
    ICommandBatchCreator &_batch_creator;
    ICommandExecutor &_executor;

    std::unique_ptr<ICommandBatch> current_block;

    void send_block();
};

#endif //BULK_INPUTPROC_H

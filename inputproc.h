//
// Created by denis on 19.12.23.
//

#ifndef BULK_INPUTPROC_H
#define BULK_INPUTPROC_H

#include <string>
#include <memory>
#include "icommand.h"

class InputProcessor
{
public:
    InputProcessor(int N, ICommandExecutor &exec, ICommandBatchCreator &batch_creator) :
        block_size(N),
        _exec(exec),
        _batch_creator(batch_creator)
    {
        current_block = std::unique_ptr<ICommandBatch>(_batch_creator.create());
    }

    ~InputProcessor() {
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
    ICommandExecutor &_exec;
    ICommandBatchCreator &_batch_creator;

    std::unique_ptr<ICommandBatch> current_block;

    void send_block() {
        if (current_block->size() > 0) {
            _exec.add_block(std::move(current_block));
            current_block = std::unique_ptr<ICommandBatch>(_batch_creator.create());
        }
    }
};

#endif //BULK_INPUTPROC_H

//
// Created by denis on 19.12.23.
//

#ifndef BULK_ICOMMAND_H
#define BULK_ICOMMAND_H

#include <memory>
#include "output.h"

using BulkTime = time_t;

/* A set of commands to be executed */
class ICommandBatch
{
public:

    virtual void execute(IOutHandlerManager &out_handlers) = 0;
    virtual BulkTime start_time() const = 0;

    virtual size_t size() const = 0;
    virtual size_t add_command(Command &&cmd) = 0;
    virtual ~ICommandBatch() = default;
};

class ICommandBatchCreator
{
public:
    virtual ICommandBatch *create() = 0;
    virtual ~ICommandBatchCreator() = default;
};

class ICommandExecutor
{
public:
    virtual void add_block(std::unique_ptr<ICommandBatch> &&block) = 0;
    virtual void execute(std::unique_ptr<ICommandBatch> &&batch) = 0;

    virtual ~ICommandExecutor() = default;
};

#endif //BULK_ICOMMAND_H

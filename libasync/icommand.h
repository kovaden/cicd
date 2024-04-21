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
    virtual void execute(IOutHandler &out_handler) = 0;
    virtual BulkTime start_time() const = 0;
    virtual bool is_stop() const = 0;

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
    virtual void add_batch(std::unique_ptr<ICommandBatch> block) = 0;
    virtual void add_handler(std::shared_ptr<IOutHandler> handler) = 0;

    virtual ~ICommandExecutor() = default;
};

#endif //BULK_ICOMMAND_H

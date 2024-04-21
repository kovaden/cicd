//
// Created by denis on 18.12.23.
//

#ifndef BULK_OUTPUT_H
#define BULK_OUTPUT_H

#include <memory>

class ICommandBatch;
class Command;

class IOutHandler
{
public:
    IOutHandler() = default;
    IOutHandler(IOutHandler &) = delete;
    IOutHandler(IOutHandler &&) = delete;

    virtual void put(std::shared_ptr<ICommandBatch> batch) = 0;
    virtual void log_batch(const ICommandBatch &, bool is_batch_end) = 0;
    virtual void log_command(const Command &) = 0;

    virtual ~IOutHandler() {};
};

class IOutHandlerManager
{
public:
    virtual void add_handler(std::shared_ptr<IOutHandler> ) = 0;

    virtual void put(std::unique_ptr<ICommandBatch> ) = 0;

    virtual ~IOutHandlerManager() =default;
};


#endif //BULK_OUTPUT_H

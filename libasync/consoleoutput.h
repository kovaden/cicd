//
// Created by denis on 19.12.23.
//

#ifndef BULK_CONSOLEOUTPUT_H
#define BULK_CONSOLEOUTPUT_H

#include "output.h"
#include "icommand.h"
#include "command.h"
#include "athread.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <queue>


class ConsoleHandler : public IOutHandler
{
public:
    ConsoleHandler() : thr(batch_queue, *this)
    {
    }

    ~ConsoleHandler() override {
        batch_queue.push(std::make_shared<CommandStop>());
        thr.stop();
    }

    static std::shared_ptr<IOutHandler> ref() {
        return instance;
    }

    void put(std::shared_ptr<ICommandBatch> batch) override {
        // put to the queue and signal receiver
        batch_queue.push(batch);
    }

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

private:

    static std::shared_ptr<IOutHandler> instance;

    std::mutex mtx;
    WaitQueue batch_queue;
    std::atomic<bool> done{false};
    AsyncProcThread thr;
};


#endif //BULK_CONSOLEOUTPUT_H

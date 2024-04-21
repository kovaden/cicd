//
// Created by denis on 19.12.23.
//

#ifndef BULK_FILEOUTPUT_H
#define BULK_FILEOUTPUT_H

#include "icommand.h"
#include "output.h"
#include "athread.h"

#include <thread>
#include <atomic>
#include <fstream>
#include <iostream>
#include <cassert>

class FileWriter : public IOutHandler
{
public:
    FileWriter() : id(++counter) {
    }

    virtual ~FileWriter() {
        if (the_file.is_open()) {
            the_file.close();
        }
    }

    void put(std::shared_ptr<ICommandBatch>) override {
        // should never be here
        assert(0);
    }
    void log_batch(const ICommandBatch &batch, bool is_batch_end) override;
    void log_command(const Command &cmd) override;

private:
    std::ofstream the_file;
    int id;
    static int counter;

    std::string getFilename(BulkTime &start) const;
};

class FileHandler : public IOutHandler
{
public:
    FileHandler() : thr1(the_queue, w1), thr2(the_queue, w2) {}

    virtual ~FileHandler();

    // Return a copy of shared pointer, making referenve count increment
    static std::shared_ptr<IOutHandler> ref() {
        return instance;
    }

    void put(std::shared_ptr<ICommandBatch> batch) override;

    void log_batch(const ICommandBatch &, bool ) override {
        assert(0);
    }

    void log_command(const Command &) override {
        assert(0);
    }
private:

    static std::shared_ptr<FileHandler> instance;

    std::mutex mtx;
    WaitQueue the_queue;

    // TODO: combine them and parametrize they amount
    AsyncProcThread thr1, thr2;    
    FileWriter w1, w2;
};


#endif //BULK_FILEOUTPUT_H

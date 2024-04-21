//
// Created by denis on 03.01.24.
//

#ifndef ASYNC_ATHREAD_H
#define ASYNC_ATHREAD_H

#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>

#include <iostream>

class WaitQueue
{
public:
    WaitQueue() {
    }

    ~WaitQueue()
    {
    }

    void push(std::shared_ptr<ICommandBatch> item) {
        {
            std::lock_guard<std::mutex> lck(cv_mutex);
            the_queue.emplace(item);
        }
        queue_cv.notify_one();
    }

    std::shared_ptr<ICommandBatch> pull_wait()
    {

        std::unique_lock<std::mutex> lck(cv_mutex);
        do {
            queue_cv.wait_for(lck, std::chrono::milliseconds(100), [this](){return !the_queue.empty();});
        } while (the_queue.empty());

        auto ret = the_queue.front();
        the_queue.pop();
        return ret;
    }

    size_t size() {
        std::unique_lock<std::mutex> lck(cv_mutex);
        return the_queue.size();
    }
private:
    std::queue<std::shared_ptr<ICommandBatch>> the_queue;
    std::condition_variable_any queue_cv;
    std::mutex cv_mutex;
};

class AsyncProcThread
{
public:
    AsyncProcThread(WaitQueue &wait_q, IOutHandler &out_h) :
            _wait_q(wait_q), thr(&AsyncProcThread::run, this), _out_handler(out_h)
    {
        std::unique_lock<std::mutex> start_wait(mtx);
        cv_started.wait(start_wait, [&st = started](){return st;});
    }

    AsyncProcThread(const AsyncProcThread &) = delete;

    ~AsyncProcThread() {
        stop();
    }

    void run() {
        {
            std::lock_guard<std::mutex> lk(mtx);
            started = true;
        }
        cv_started.notify_all();

        while (true) {
            auto b = _wait_q.pull_wait();

            if (b->is_stop()) {
                break;
            }

            b->execute(_out_handler);
        }
    }

    void stop() {
        if (thr.joinable()) {
            thr.join();
        }
    }
private:
    std::mutex mtx;
    std::condition_variable cv_started;
    bool started{false};
    WaitQueue &_wait_q;
    std::thread thr;
    IOutHandler &_out_handler;
};

#endif //ASYNC_ATHREAD_H

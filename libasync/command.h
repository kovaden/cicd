#pragma once

#include <vector>
#include <ctime>
#include <string>
#include <cassert>
#include <iostream>

#include "icommand.h"
#include "output.h"
#include <cassert>

int version();

/* Single command to be executed */
class Command
{
	public:
        Command(const std::string &s) : text(s){
            time(&_start_time);
        }

        void execute() {
            /*
             * Do something really important here....
             */
        }

        const std::string &str() const {
            return text;
        }

        BulkTime start_time() const {
            return _start_time;
        }
private:
        std::string text;
        BulkTime _start_time;
};

class CommandStop : public ICommandBatch
{
public:
    CommandStop() = default;
    ~CommandStop() override = default;

    void execute(IOutHandler &) override {
        assert(false);
    }

    bool is_stop() const override {
        return true;
    }

    BulkTime start_time() const override {
        assert(false);
    }

    size_t size() const override{
        return 0;
    }

    size_t add_command(Command &&) override {
        assert(false);
    }

};

class CommandBlock;

class BlockLogContext
{
public:
    BlockLogContext(ICommandBatch &cmdblk, IOutHandler &out_handler) : _cmdblk(cmdblk), _out_handler(out_handler) {
        _out_handler.log_batch(_cmdblk, false);
    }

    ~BlockLogContext() {
        _out_handler.log_batch(_cmdblk, true);
    }
private:
    ICommandBatch &_cmdblk;
    IOutHandler &_out_handler;
};

class CommandBlock : public ICommandBatch
{
	public:
        CommandBlock() = default;
        ~CommandBlock() override = default;

        void execute(IOutHandler &out_handler) override {
            BlockLogContext ctx(*this, out_handler);

            for (auto cmd : commands) {
                out_handler.log_command(cmd);
                cmd.execute();
            }
        }

        bool is_stop() const override {
            return false;
        }

        BulkTime start_time() const override {
            assert(!commands.empty());
            return commands[0].start_time();
        }

        size_t size() const override{
            return commands.size();
        }

        size_t add_command(Command &&cmd) override {
            commands.emplace_back(cmd);
            return commands.size();
        }

	private:
		std::vector<Command> commands;
};

class CommandExecutor : public ICommandExecutor
{
public:

    void add_batch(std::unique_ptr<ICommandBatch> block) override {
        // Put the commant into handler's queue
        _out_handlers->put(std::move(block));
    }

    void add_handler(std::shared_ptr<IOutHandler> handler) override;

    explicit CommandExecutor();

    virtual ~CommandExecutor() = default;
private:
    std::unique_ptr<IOutHandlerManager> _out_handlers;
};

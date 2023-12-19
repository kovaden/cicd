#pragma once

#include <vector>
#include <ctime>
#include <string>
#include <cassert>
#include <iostream>

#include "icommand.h"
#include "output.h"

int version();

/* Single command to be executed */
class Command
{
	public:
        Command(const std::string &s) : text(s){
            time(&_start_time);
        }
        void execute() {
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


class CommandBlock : public ICommandBatch
{
	public:
        CommandBlock() = default;
        ~CommandBlock() override = default;

        void execute(IOutHandlerManager &out_handlers) override {
            for (auto cmd : commands) {
                out_handlers.log_command(cmd);
                cmd.execute();
            }
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

    void add_block(std::unique_ptr<ICommandBatch> &&block) override {
        execute(std::move(block));
    }

    explicit CommandExecutor(IOutHandlerManager &out_handlers) : _out_handlers(out_handlers) {}

    void execute(std::unique_ptr<ICommandBatch> &&batch) override {
        _out_handlers.log_batch(*batch, false);
        batch->execute(_out_handlers);
        _out_handlers.log_batch(*batch, true);
    }

    virtual ~CommandExecutor() {}
private:
    IOutHandlerManager &_out_handlers;
};
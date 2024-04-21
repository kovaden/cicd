//
// Created by denis on 03.01.24.
//

#include "command.h"
#include "consoleoutput.h"
#include "fileoutput.h"


class OutHandlerManager : public IOutHandlerManager
{
public:
    OutHandlerManager() = default;
    ~OutHandlerManager() override = default;

    void add_handler(std::shared_ptr<IOutHandler> h) override {
        handlers.emplace_back(std::move(h));
    }

    void put(std::unique_ptr<ICommandBatch> b) override
    {
        // We need to share command batch between handlers here
        std::shared_ptr<ICommandBatch> sb(std::move(b));

        for (auto &h : handlers) {
            h->put(sb);
        }
    }

private:
    std::vector<std::shared_ptr<IOutHandler>> handlers;
};


CommandExecutor::CommandExecutor()
{
    _out_handlers = std::make_unique<OutHandlerManager>();

    add_handler(ConsoleHandler::ref());
    add_handler(FileHandler::ref());
}

void CommandExecutor::add_handler(std::shared_ptr<IOutHandler> handler_ref)
{
    _out_handlers->add_handler(handler_ref);
}



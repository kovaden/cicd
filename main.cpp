#include "command.h"
#include "inputproc.h"
#include "fileoutput.h"
#include "consoleoutput.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

static const int N_DEFAULT = 3;
using namespace std;



class OutHandlerManager : public IOutHandlerManager
{
public:
    void add_handler(std::shared_ptr<IOutHandler> h) override {
        handlers.emplace_back(h);
    }

    void log_batch(const ICommandBatch &b, bool is_batch_end) override {
        for (auto &h : handlers) {
            h->log_batch(b, is_batch_end);
        }
    }

    void log_command(const Command &cmd) override {
        for(auto &h : handlers) {
            h->log_command(cmd);
        }
    }
private:
    std::vector<std::shared_ptr<IOutHandler>> handlers;
};

class CommandBlockCreator : public ICommandBatchCreator
{
public:
    ICommandBatch *create() override {
        return new CommandBlock;
    }

    virtual ~CommandBlockCreator() = default;
};

void usage() {
    cout << "Bulk command processing. Usage:" << endl;
    cout << "    07-bulk <N>" << endl;
    cout << "  where N is batch size, default is " << N_DEFAULT << endl;
}

int main (int argc , char * argv[])
{
    string command;

    int N = 0;

    if (argc == 1) {
        N = N_DEFAULT;
    } else if (argc == 2) {
         try {
             N = std::stoi(argv[1]);
         } catch (std::invalid_argument &) {}
    }

    if (N == 0) {
        usage();
        exit(1);
    }

    OutHandlerManager handlerManager;

    handlerManager.add_handler(make_shared<ConsoleHandler>());
    handlerManager.add_handler(make_shared<FileHandler>());

    auto exec = CommandExecutor(handlerManager);

    CommandBlockCreator block_creator;

    InputProcessor cp(N, exec, block_creator);

    std::cout << "Bulk command processor. Version: " << version() << std::endl;

    while (cin) {
        cin >> command;
        cp.process(command);
    }

    return 0;
}

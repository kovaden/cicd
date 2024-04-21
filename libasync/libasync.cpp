
#include "version.h"
#include "async.h"
#include "command.h"
#include "inputproc.h"

#include <cstdint>
#include <stdexcept>
#include <sstream>

static const int N_DEFAULT = 3;

int version() {
    return my_version; // PROJECT_VERSION_PATCH;
}

const uint32_t MAGIC_BYTES = 0xCAFEBEEF;

class CommandBlockCreator : public ICommandBatchCreator
{
public:
    CommandBlockCreator() = default;
    ICommandBatch *create() override {
        return new CommandBlock;
    }

    ~CommandBlockCreator() override = default;
};

class Context {
    public:
        class invalid_context : public std::invalid_argument
        {
        public:
            invalid_context() : std::invalid_argument("Context is invalid") {}
        };

        explicit Context(int bulk_size=N_DEFAULT) :  cp(bulk_size, block_creator, ce) {
        }

        virtual ~Context() {
        }

        void process(const std::string &cmd) {
            dbg_check();
            cp.process(cmd);
        }

        /*
         * It's bad practice to throw exception from the destructor, so implement this destroyer
         */
        void destroy() {
            dbg_check();
            delete this;
        }

        /*
         * Simple check that this is a valid instance
         * Failure in this check may mean that invalid context_t pointer was used
         */
        void dbg_check() {
            if (magic != MAGIC_BYTES) {
                throw invalid_context();
            }
        }

private:
        uint32_t magic {MAGIC_BYTES};

        CommandBlockCreator block_creator;
        CommandExecutor ce;
        CommandProcessor cp;
};

async::context_t async::connect(std::size_t bulk)
{
    return static_cast<async::context_t>(new Context(bulk));
}

void async::receive(async::context_t handle, const char *data, std::size_t size)
{
    Context *context = static_cast<Context *>(handle);

    std::string input(data, size);

    std::istringstream iss(input);
    std::string command;

    while(std::getline(iss, command,'\n')) {
        context->process(command);
    }
}

void async::disconnect(async::context_t handle)
{
    Context *context = static_cast<Context *>(handle);
    context->destroy();
}

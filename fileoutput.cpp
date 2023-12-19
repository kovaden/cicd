#include "fileoutput.h"
#include "command.h"

#include <sstream>

using namespace std;

void FileHandler::log_batch(const ICommandBatch &batch, bool is_batch_end)
{
    // Open a new file for the batch

    if (is_batch_end) {
        the_file << endl;
        the_file.close();
        return;
    }

    if (the_file.is_open()) {
        the_file.close();
    }

    auto start = batch.start_time();

    stringstream sfname;
    sfname << "bulk" << start << ".log";
    the_file.open(sfname.str());
}

void FileHandler::log_command(const Command &cmd)
{
    the_file << cmd.str() << endl;
}

#include "fileoutput.h"
#include "command.h"

#include <sstream>

using namespace std;

int FileWriter::counter{0};

void FileWriter::log_batch(const ICommandBatch &batch, bool is_batch_end)
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

    string fname = getFilename(start);

    the_file.open(fname);
}

string FileWriter::getFilename(BulkTime &start) const {
    struct tm ltm;
    auto plt = localtime_r(&start, &ltm);

    stringstream sfname;
    sfname << "bulk_" << 1900 + plt->tm_year << "_" << 1 + plt->tm_mon << "_" << plt->tm_mday << "_"
           << plt->tm_hour << "_" << plt->tm_min << "_" << plt->tm_sec << "_" << id << ".log";
    auto fname = sfname.str();
    return fname;
}

void FileWriter::log_command(const Command &cmd)
{
    the_file << cmd.str() << endl;
}

std::shared_ptr<FileHandler> FileHandler::instance = std::make_shared<FileHandler>();

void FileHandler::put(std::shared_ptr<ICommandBatch> batch)
{
    the_queue.push(batch);
}

FileHandler::~FileHandler() {
    put(std::make_shared<CommandStop>());
    put(std::make_shared<CommandStop>());

    thr1.stop();
    thr2.stop();
}

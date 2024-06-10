#include "server.h"

/* 
use boost::program_options to parse command line. It seems to be an overhead for the simple 2 values,
but this project is educational, so it good to learn how to use it.
 */
#include <boost/program_options.hpp>

namespace po = boost::program_options;


int main(int argc, char *argv[]) 
{
    server_params params;

    po::positional_options_description p;

    p.add("port", 1).add("bulk_size", 1);

    po::options_description desc("Allowed options");
    desc.add_options() 
        ("port", po::value<int>(&params.port)->default_value(9000), "port to start server on");

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);


    // std::size_t bulk = vm["bulk_size"].as<std::size_t>();
    // int port = vm["port"].as<int>();

    boost::asio::io_context io_context;
    server s(io_context, params);
    io_context.run();

    return 0;
}


#include "lib.h"
#include <iostream>

#include <boost/program_options.hpp>

#include "bayanlib/BayanSearcher.h"
#include "bayanlib/ProgramOptions.h"

namespace po = boost::program_options;

using namespace std;

enum HashAlgorithm {CRC32, MD5};

void run_searcher(int argc, char *const *argv) {
    po::options_description desc{"Options"};

    desc.add_options()
            ("help,h", "This text")
            ("include,i", boost::program_options::value<vector<string>>()->multitoken(),
                    "Scan directory (may appear several times)")
            ("exclude,e", boost::program_options::value<vector<string>>()->multitoken(),
                    "Skip (exclude) directory (may appear several times)")
            ("depth", po::value<int>()->default_value(-1),
                    "scanning depth, default is unlimited")
            ("min_size", po::value<int>()->default_value(1),
                    "Minimum file size for scanning. Default is 1")
            ("filemask", boost::program_options::value<string>()->default_value(""), "File mask")
            ("block_size", po::value<int>()->default_value(4096),
                    "Block size to read files")
            ("hash_alg", boost::program_options::value<string>()->default_value("CRC32"),
             "Hash algorithm used to compare files. Supported algorihms: CRC32 MD5")
            ;

    po::variables_map vm;
    po::store(parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << "Usage: " << argv[0] << " [options] " << endl;
        cout << desc << endl;
        return;
    }

    BayanSearcher searcher;

    if (vm.count("include") > 0) {
        for (auto inc_path : vm["include"].as<vector<string>>() ) {
            searcher.include(inc_path);
        }
    } else {
        searcher.include(".");
    }

    if (vm.count("exclude") > 0) {
        for (auto excl_path : vm["exclude"].as<vector<string>>() ) {
            searcher.exclude(excl_path);
        }
    }

    ProgramOptions::block_size(vm["block_size"].as<int>());
    ProgramOptions::hash(vm["hash_alg"].as<string>());

    auto d = vm["depth"].as<int>();
    auto f = vm["filemask"].as<string>();
    auto m = vm["min_size"].as<int>();

    searcher.depth(d).
        file_mask(f).
        min_size(m);

    searcher.search();

    searcher.report(std::cout);
}

int main (int argc, char *argv[]) {
    try {
        run_searcher(argc, argv);
    } catch (std::exception &ex) {
        cout << "Error: " << ex.what() << endl;
        return 1;
    }

    return 0;
}

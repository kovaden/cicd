//
// Created by denis on 22.12.23.
//

#ifndef HELLOWORLD_BAYANSEARCHER_H
#define HELLOWORLD_BAYANSEARCHER_H

#include "MatchGroup.h"
#include "FileHash.h"
#include <string>
#include <unordered_map>
#include <boost/filesystem/path.hpp>
#include "boost/regex.hpp"

class BayanSearcher {
public:
    BayanSearcher &include(const std::string &path);
    BayanSearcher &exclude(const std::string &path);
    BayanSearcher &depth(int new_depth);
    BayanSearcher &min_size(int new_size);
    BayanSearcher &file_mask(const std::string &mask);

    std::vector<MatchGroup> search();

    void report(std::ostream &out);
private:
    void process_directory(const boost::filesystem::path &dir, int level);
    void process_file(const boost::filesystem::path &dir);

    std::vector<std::string> include_dirs;
    std::vector<std::string> exclude_dirs;

    bool is_directory_excluded(const std::string &dir) {
        return std::any_of(exclude_dirs.begin(), exclude_dirs.end(),
                           [&dir](const auto &s) { return s == dir;});
    }

    int maxdepth{0};
    size_t minsize{1};

    boost::regex filter;
    bool use_filter{false};

    /*
     *   Double hash will allow more duplicates but should dramatically decrease search time
     */
    std::list<std::unique_ptr<MatchGroup>> matches;
    std::list<std::unique_ptr<BayanFile>> unmatched;
};


#endif //HELLOWORLD_BAYANSEARCHER_H

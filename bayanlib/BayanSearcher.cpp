//
// Created by denis on 22.12.23.
//

#include "BayanSearcher.h"
#include "HashFactory.h"

#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
#include <algorithm>
#include <iostream>

namespace fs = boost::filesystem;
using namespace std;

BayanSearcher &BayanSearcher::include(const std::string &path)
{
    include_dirs.emplace_back(path);
    return *this;
}

BayanSearcher &BayanSearcher::exclude(const std::string &path)
{
    exclude_dirs.emplace_back(path);
    return *this;
}

BayanSearcher &BayanSearcher::depth(int new_depth)
{
    maxdepth = new_depth;
    return *this;
}

BayanSearcher &BayanSearcher::min_size(int new_size) {
    minsize = new_size;
    return *this;
}

BayanSearcher &BayanSearcher::file_mask(const std::string &new_mask) {
    if (!new_mask.empty()) {
        filter = boost::regex(new_mask);
        use_filter = true;
    } else {
        use_filter = false;
    }
    return *this;
}

std::vector<MatchGroup> BayanSearcher::search() {
    std::vector<MatchGroup> ret;

    for (fs::path p : include_dirs) {
        if (is_directory(p)) {
            process_directory(p.string(), 0);
        } else if (is_regular_file(p)) {
            process_file(p);
        }
    }

    return ret;
}

void BayanSearcher::process_directory(const boost::filesystem::path &dir, int level)
{
    if (maxdepth != -1 && level > maxdepth) {
        return;
    }

    if (is_directory_excluded(dir.string())) {
        return;
    }

    for (fs::path p : fs::directory_iterator(dir)) {
        if (is_directory(p)) {
            process_directory(p, level + 1);
        } else if (is_regular_file(p)) {
            process_file(p);
        }
    }
}

void BayanSearcher::process_file(const boost::filesystem::path &p)
{
    auto sz = fs::file_size(p);
    if (sz < minsize) {
        return;
    }

    if (use_filter) {
        boost::smatch what;
        if (!boost::regex_match(p.filename().string(), what, filter)) {
            return;
        }
    }

    auto f = std::make_unique<BayanFile>(p);

    auto match_found = std::find_if(matches.begin(), matches.end(),
                                        [&f](auto &match_group)
                                        {
                                                    return match_group->equals(*f.get());
                                        }
                                    );

    if (match_found != matches.end()) {
       (*match_found)->AddFile(std::move(f));
//        cout << "File " << p.string() << " added to match" << endl;
        return;
    }

    auto file_found = std::find_if(unmatched.begin(), unmatched.end(),
                                   [&f](auto &ufile)
                                   {return ufile->equal(*f.get());});

    if (file_found != unmatched.end()) {
        /* new match */
        auto match = std::make_unique<MatchGroup>(std::move(*file_found), std::move(f));
        matches.emplace_back(std::move(match));
        unmatched.erase(file_found);
//        cout << "File " << p.string() << " match created " << endl;
        return;
    }

    unmatched.emplace_back(std::move(f));
}

void BayanSearcher::report(ostream &out)
{
    if (matches.empty()) {
        out << "No matches found" << endl;
        return;
    }

    for (auto &m : matches) {
        out << *m << endl;
    }
}

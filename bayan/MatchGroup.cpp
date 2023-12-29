//
// Created by denis on 22.12.23.
//

#include "MatchGroup.h"

MatchGroup::MatchGroup(std::unique_ptr<BayanFile> &&first, std::unique_ptr<BayanFile> &&second)
{
    matches.emplace_back(std::move(first));
    matches.emplace_back(std::move(second));
}

bool MatchGroup::equals(BayanFile &other)
{
    for (auto &m: matches) {
        if (m->equal(other))
            return true;
    }
    return false;
}

bool MatchGroup::AddFile(std::unique_ptr<BayanFile> &&another) {
    matches.emplace_back(std::move(another));
    return false;
}

std::ostream & operator << (std::ostream &os, MatchGroup &mg)
{
    os << "Match of " << mg.matches.size() << " found:" << std::endl;
    for (const auto &m : mg.matches) {
        os << m.get()->name() << std::endl;
    }
    return os;
}

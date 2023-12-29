//
// Created by denis on 22.12.23.
//

#ifndef HELLOWORLD_MATCHGROUP_H
#define HELLOWORLD_MATCHGROUP_H

#include <vector>
#include "BayanFile.h"

/* reptesent group of identical files */
class MatchGroup {
public:
    MatchGroup(std::unique_ptr<BayanFile> &&first, std::unique_ptr<BayanFile> &&second);
    bool equals(BayanFile &other);

    bool AddFile(std::unique_ptr<BayanFile> &&another);

    friend std::ostream & operator << (std::ostream &, MatchGroup &);
private:
    std::vector<std::unique_ptr<BayanFile>> matches;
};

std::ostream & operator << (std::ostream &, MatchGroup &);

#endif //HELLOWORLD_MATCHGROUP_H

//
// Created by denis on 24.09.23.
//

#include <iostream>
#include <map>

#include "myalloc.h"
#include "mycont.h"

using namespace std;

int fact(int x)
{
    int ret = x;
    while (x-- > 1)
        ret *= x;

    return ret;
}

int main()
{
    map<int, int> stdmap;
    map<int, int, std::less<int>, ElementAllocator<std::pair<int, int> , 10 > > stdmap_ea;

    for (int i = 1; i < 10; i++) {
        stdmap[i] = fact(i);
        stdmap_ea[i] = fact(i);
    }

    for (auto it : stdmap) {
        cout << it.first << " " << it.second << endl;
    }

    for (auto it : stdmap_ea) {
        cout << it.first << " " << it.second << endl;
    }

    mycont<int> mycont_int;

    mycont<int, ElementAllocator<int, 10>> mycont_ea;

    for (int i = 0; i < 10; i++) {
        mycont_int.append(i);
        mycont_ea.append(i);
    }

    for (auto it : mycont_int) {
        cout << it << " ";
    }
    cout << endl;

    for (auto it : mycont_ea) {
        cout << it << " ";
    }
    cout << endl;

    return 0;
}

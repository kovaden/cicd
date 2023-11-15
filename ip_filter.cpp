#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "ip_filter.h"


// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));
    return r;
}

std::ostream & operator <<(std::ostream &out, const ip_addr &ip)
{
    for(auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part) {
        if (ip_part != ip.cbegin()) {
            std::cout << ".";
        }
        std::cout << static_cast<int>(*ip_part);
    }

    return out;
}

std::ostream & operator <<(std::ostream &out, const std::vector<ip_addr> &ip_pool)
{
    for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip) {
        std::cout << *ip << std::endl;
    }
    return out;
}

ip_addr str2ip(const std::vector<std::string> &s)
{
    ip_addr ret;
    assert(s.size() == 4);
    for (int i = 0; i < 4; i++) {
        ret[i] = std::stoi(s[i]);
    }
    return ret;
}

void revsort(std::vector<ip_addr> &ip_pool)
{
    std::sort(ip_pool.begin(), ip_pool.end(),
          [](const ip_addr &a1, const ip_addr &a2){
        auto it1 = a1.cbegin();
        auto it2=a2.cbegin();

        for (; it1 != a1.cend() && it2 != a2.cend(); it1++, it2++) {
            if (*it1 != *it2) {
                return *it1 > *it2;
            }
        }
        return false;
    });
}

std::vector<ip_addr> filter(const std::vector<ip_addr> &ip_pool, int key)
{
    std::vector<ip_addr> ret;
    std::copy_if(ip_pool.cbegin(), ip_pool.cend(), std::back_inserter(ret),
                 [key](const ip_addr &addr) {
                     return addr[0] == key;
                 });
    return ret;
}

std::vector<ip_addr> filter(const std::vector<ip_addr> &ip_pool, const std::tuple<int, int> &key)
{
    std::vector<ip_addr> ips;
    std::copy_if(ip_pool.cbegin(), ip_pool.cend(), std::back_inserter(ips),
             [key](const ip_addr &addr) {
                     return addr[0] == std::get<0>(key) && addr[1] == std::get<1>(key);
                 });
    return ips;
}

std::vector<ip_addr> filter_any(const std::vector<ip_addr> &ip_pool, int my_byte)
{
    std::vector<ip_addr> ret;
    std::copy_if(ip_pool.cbegin(), ip_pool.cend(), std::back_inserter(ret),
             [my_byte](const ip_addr &addr) {
                     return std::find(addr.cbegin(), addr.cend(), my_byte) != addr.cend();
                 });
    return ret;
}


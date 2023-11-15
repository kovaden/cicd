//
// Created by denis on 24.09.23.
//

#ifndef IP_FILTER_IP_FILTER_H
#define IP_FILTER_IP_FILTER_H

#include <array>
#include <vector>
#include <string>
#include <tuple>

using ip_addr = std::array<std::uint8_t , 4>;

std::vector<std::string> split(const std::string &str, char d);
ip_addr str2ip(const std::vector<std::string> &s);

void revsort(std::vector<ip_addr> &ip_pool);
std::vector<ip_addr> filter(const std::vector<ip_addr> &ip_pool, int key);
std::vector<ip_addr> filter(const std::vector<ip_addr> &ip_pool, const std::tuple<int, int> &key);
std::vector<ip_addr> filter_any(const std::vector<ip_addr> &ip_pool, int my_byte);

std::ostream & operator <<(std::ostream &out, const std::vector<ip_addr> &ip_pool);

#endif //IP_FILTER_IP_FILTER_H

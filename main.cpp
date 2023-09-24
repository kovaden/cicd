//
// Created by denis on 24.09.23.
//

#include <iostream>

#include "ip_filter.h"

int main(int argc, char const *argv[]) {
    (void)argc;
    (void)argv;

    try {
        std::vector<ip_addr> ip_pool;

        for(std::string line; std::getline(std::cin, line);)  {
            std::vector<std::string> v = split(line, '\t');

            auto sip =split(v.at(0), '.');
            ip_pool.push_back(str2ip(sip));
        }

        // reverse lexicographically sort
        revsort(ip_pool);
        std::cout << ip_pool;

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // filter by first byte and output
        // ip = filter(1)

        std::vector<ip_addr> ip = filter(ip_pool, 1);
        std::cout << ip;

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // filter by first and second bytes and output
        // ip = filter(46, 70)
        auto filtered_2 = filter(ip_pool, std::make_tuple(46, 70));

        std::cout << filtered_2;

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // filter by any byte and output
        // ip = filter_any(46)
        std::vector<ip_addr> filtered_any = filter_any(ip_pool, 46);
        std::cout << filtered_any;

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

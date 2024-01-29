/*
 * На вход подаётся тестовый файл F, в котором каждая строка содержит адрес электронной почты.
 * Выбрать минимальное число букв для однозначного определения слова.
 */

#include "mapreduce.h"

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

void usage() {
    cout << "MapReduce sample system. Usage:" << endl;
    cout << "  mapreduce <infile> <NMappers> <NReducers>";

}

class Mapper : public mapreduce::IMapper
{
public:
    Mapper(mapreduce::mr_chunk &objects) : _chunk(objects) {}

    void operator() (mapreduce::mr_object &obj) override {
        _chunk.emplace_back(obj);
    }

    void at_map_done() override {
        cout << "Sorting " << _chunk.size() << " elements..." << endl;
        std::sort(_chunk.begin(), _chunk.end(),
                  [](mapreduce::mr_object &o1, mapreduce::mr_object &o2){ return o1 < o2;}
                  );
    }

private:
    mapreduce::mr_chunk &_chunk;
};

/* return max number of strings with given prefix len */
int reducer (mapreduce::mr_chunk &chunk)
{
    // Maximum count of names starting with specified prefix
    int max_count = 0;
    mapreduce::mr_object max_value;

    int current_prefix = 0;

    do {
        max_count = 0;
        current_prefix ++;

        auto it = chunk.begin();
        mapreduce::mr_object current_value = it->substr(0, current_prefix);

        int current_count = 0;

        cout << "Checking prefix length..." << current_prefix << endl;

        while ((it != chunk.end())) {

            if (it->substr(0, current_prefix) == current_value) {
                current_count++;
            } else {
                if (current_count > max_count) {
                    max_count = current_count;
                    max_value = current_value;
                    current_count = 0;
                }
                current_value = it->substr(0, current_prefix);
            }
            it++;
        }

        cout << "Prefix:" << current_prefix << " max names:" << max_count << " max name: " << max_value << endl;

    } while (max_count > 1);

    return current_prefix;
}

class MapFactory : public mapreduce::IMapFactory
{
public:
    std::unique_ptr<mapreduce::IMapper> createMapper(mapreduce::mr_chunk &objects) override {
        return make_unique<Mapper>(objects);
    }
};


int main (int argc, char *argv[])
{
    if (argc != 4) {
        usage();
        return 1;
    }

    try {
        int nmappers = stoi(argv[2]);
        int nreducers = stoi(argv[3]);

        MapFactory factory;

        int min_prefix = mapreduce::do_mapreduce(&factory, argv[1], nmappers, nreducers, reducer);

        std::cout << "Minimal prefix len = " <<  min_prefix << endl;

    } catch (std::exception &ex) {
        cout << "Exception:" << endl;
        cout << ex.what() << endl;
        return 2;
    }
    return 0;
}

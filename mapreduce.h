#pragma once

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

namespace mapreduce {

    using mr_object = std::string ;
    using mr_chunk = std::vector<mr_object>;
    using chunk_iterator = std::vector<mapreduce::mr_object>::iterator;

    class IMapper
    {
    public:
        virtual void operator() (mr_object &obj) = 0;
        virtual void at_map_done() = 0;
        virtual ~IMapper() = default;
    };

    /*
     *  IMapper is going to have a state
     */

    class IMapFactory
    {
    public:
        virtual std::unique_ptr<IMapper> createMapper(mr_chunk &objects) = 0;
    };

    int do_mapreduce(IMapFactory *mr_factory, const char *input_file, int nmappers, int nreducers,
                     std::function<int(mapreduce::mr_chunk &)> reducer);
};

#include "mapreduce.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <array>
#include <list>

using namespace mapreduce;
using namespace std;

void mapper_thread(IMapper &mapper, const char *input_file, streamoff start_pos, streamoff end_pos)
{
    ifstream ifs(input_file);

    // Find nearest EOL to start from the beginning
    if (start_pos > 0) {
        // Go to previous symbol, if it's EOL, then start_pos is on the beginning of line
        ifs.seekg(start_pos - 1);

        char sym;
        ifs >> sym;
        if (sym != '\n') {
            array<char, 256> buf;
            // Middle of line, read to it's end. Take next one as a start
            ifs.getline(buf.data(), buf.size());
        }
    }

//    ifs positioned to begin of line for this mapper. Read lines till end
    string line;
    while (getline(ifs, line)) {
        if (ifs.tellg() > end_pos) {
            break;
        }
        mapper(line);
    }

    mapper.at_map_done();
}

/*
 * Общая задача на этапе смешивания – переместить строки из M контейнеров (результат этапа map) в R контейнеров
 * (входные данные для этапа reduce). При этом сделать это нужно таким образом., чтобы одинаковые данные
 *  попали в один и тот же контейнер для свёртки.
 *
 *  Это можно делать в R потоков, каждый поток заполняет свой контейнер
 */
void shuffle_thread(vector<mr_chunk> &map_chunks, mr_chunk &reduce_chunk, char cmin, char cmax)
{
    // Perform parallel scan on all chunks
    // We will keep M iterators and forward then syncronously while processing next element
    using iter_range = std::pair<mapreduce::chunk_iterator, mapreduce::chunk_iterator>;
    std::list<iter_range> iter_list;

    string min_prefix = string {cmin};

    // Find starting element for this reducer in M chunks
    for (auto &chunk : map_chunks) {
        //  chunks are sorted, use binary search to find first element to start reduce
        mapreduce::chunk_iterator f = std::lower_bound(chunk.begin(), chunk.end(), min_prefix);
        if (f == chunk.end()) {
            continue;
        }
        iter_list.emplace_back(f, chunk.end());
    }

    // While there are unprocessed elements...

    cout << "Shuffle " << map_chunks.size() << " chunks...";
    while (iter_list.size() > 0) {

        // Find min object pointed by iterators. This will keep objects sorted
        auto min_it = std::min_element(iter_list.begin(), iter_list.end(),
                                      [](iter_range &it1, iter_range &it2) {return *it1.first < *it2.first;});

        reduce_chunk.push_back(*min_it->first);

        min_it->first++;
        // Remove iterators when we are finished
        iter_list.remove_if([cmax](auto it){
            return (it.first == it.second) || ((*it.first)[0] > cmax);
        });
    }
}

int mapreduce::do_mapreduce(IMapFactory *mr_factory, const char *input_file, int nmappers, int nreducers,
                            std::function<int(mapreduce::mr_chunk &)> reducer)
{
//    split file to M parts
    ifstream ifs(input_file);
    if (!ifs.is_open()) {
        throw invalid_argument("could not open input file");
    }

    ifs.seekg(0, ios_base::end);
    auto fsize = ifs.tellg();
    auto chunk = fsize / nmappers;

    // Map step

    vector<thread> map_threads;
    vector<mr_chunk> map_chunks(nmappers);
    vector<unique_ptr<IMapper>> mapers;

    for (auto m = 0; m < nmappers; m++) {
        auto start_pos = m * chunk;
        auto end_pos = (m+1)*chunk;

        mapers.emplace_back(mr_factory->createMapper(map_chunks[m]));

        map_threads.emplace_back(mapper_thread, std::ref(*mapers[m].get()), input_file, start_pos, end_pos);
    }

    for (auto &mt : map_threads) {
        mt.join();
    }

    // Shuffle step

    vector<thread> shuffle_threads;
    vector<mr_chunk> reduced_chunks(nreducers);

    int letter_per_reducer = ('Z' - 'A') / nreducers;

    for (auto r = 0; r < nreducers; r++) {
        char cmin = 'A' + r*nreducers;
        char cmax = cmin + letter_per_reducer;

        shuffle_threads.emplace_back(shuffle_thread, std::ref(map_chunks), std::ref(reduced_chunks[r]), cmin, cmax);
    }

    for (auto &st : shuffle_threads) {
        st.join();
    }

    // Do reduce with user-provided reducer

    // We have R sorted arrays of objects, start reducers to find minimal prefix for each of them.
    // Maximum value will be an answer
    vector<thread> reduce_threads;

    vector<int> reduce_results(nreducers);

    for (auto r = 0; r < nreducers; r++) {

        reduce_threads.emplace_back(
                [&reduced_chunks, &reduce_results, reducer](auto r){
                        reduce_results[r] = reducer(reduced_chunks[r]);
                    }, r);
    }

    for (auto &rt : reduce_threads) {
        rt.join();
    }

    auto result = std::max_element(reduce_results.begin(), reduce_results.end());

    return *result;
}


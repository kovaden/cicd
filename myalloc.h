//
// Created by denis on 24.09.23.
//

#ifndef IP_FILTER_MYALLOC_H
#define IP_FILTER_MYALLOC_H

#include <array>
#include <vector>
#include <list>
#include <string>
#include <tuple>
#include <bitset>

#include <assert.h>

template <typename T, int count>
class ElementAllocatorChunk
{
public:
    static const size_t BAD_POS = (size_t)-1;

    ElementAllocatorChunk() {
        _mem = reinterpret_cast<T *>(::malloc(sizeof (T) * count));
        if (_mem == nullptr) {
            throw std::bad_alloc();
        }
    }

    ElementAllocatorChunk(const ElementAllocatorChunk &) = delete;
    const   ElementAllocatorChunk &operator=(ElementAllocatorChunk &)  = delete;
    const   ElementAllocatorChunk &operator=(ElementAllocatorChunk &&)  = delete;

    ElementAllocatorChunk(ElementAllocatorChunk &&other)
    {
        busy_map = other.busy_map;
        _mem = other._mem;
        other._mem = nullptr;
    }

    ~ElementAllocatorChunk() {
        if (_mem != nullptr) {
            ::free(_mem);
        }
    }

    size_t find_map_window(std::size_t window_len) {
        for (size_t i = 0; i <= count - window_len; i++) {
            size_t j;

            for (j = i; j < count; j++) {
                if (busy_map.test(j)) {
                    break;
                }
                if (j - i == window_len-1) {
                    return i;
                }
            }
        }
        // Not found - no free window of requested size
        return BAD_POS;
    }

    T *allocate(size_t n, size_t pos=BAD_POS) {
        // Find N sequential bits in the mask
        if (pos == BAD_POS) {
            pos = find_map_window(n);
        }

        if (pos == BAD_POS) {
            throw std::bad_alloc();
        }

        // Mark allocated blocks as busy
        for (size_t i = pos; i < pos+n; i++) {
            busy_map.set(i);
        }
        return &_mem[pos];
    }

    bool has_p(T *p) {
        return p >= _mem && p <= (_mem + count);
    }

    void free(T *p, size_t n) {
        while(n--) {
            int i = p - _mem;
            assert(busy_map.test(i));
            busy_map.reset(i);
            p++;
        }
    }


private:
    std::bitset<count> busy_map{0};
    T *_mem = nullptr;
};

template <typename T, int count>
class ElementAllocator
{
public:

    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;

    typedef ElementAllocatorChunk<T, count> eac;

    ElementAllocator()
    {
    }

    T* allocate (std::size_t n)
    {
        if (n > count) {
            throw std::bad_alloc();
        }

        size_t pos = eac::BAD_POS;
        auto use_chunk = _chunks.begin();

        auto it = _chunks.begin();
        for (; it != _chunks.end(); it++) {
            pos = it->find_map_window(n);
            if (pos != eac::BAD_POS) {
                use_chunk = it;
                break;
            }
        }

        if (pos == eac::BAD_POS) {
            _chunks.emplace_back(eac());
            use_chunk = _chunks.end();
            use_chunk--;
        }
        return use_chunk->allocate(n);
    }

    void deallocate (T* p, std::size_t n)
    {
        // Find a chuck with this elem
        for (auto it = _chunks.begin(); it != _chunks.end(); it++) {
            if (it->has_p(p)) {
                it->free(p, n);
                return;
            }
        }

        throw std::bad_alloc();
    }

    template< class U >
    struct rebind
    {
        typedef ElementAllocator<U, count> other;
    };
    size_t nchunks() {
        return _chunks.size();
    }
private:
    std::list<ElementAllocatorChunk<T, count>> _chunks;
};

#endif //IP_FILTER_MYALLOC_H

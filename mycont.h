//
// Created by denis on 27.09.23.
//

#ifndef MYALLOC_MYCONT_H
#define MYALLOC_MYCONT_H

#include <memory>

template <typename T, typename Allocator = std::allocator<T>, size_t my_size=10>
class mycont {
public:

    class iterator {
    public:
        iterator(T* pos) : _pos(pos) {}

        void operator++() {
            _pos++;
        }

        T& operator *() {
            return *_pos;
        }

        bool operator == (const iterator &other) {
            return _pos == other._pos;
        }

        bool operator != (const iterator &other) {
            return _pos != other._pos;
        }

    private:
        T *_pos;
    };

    mycont() {
        _mem = allocator.allocate(my_size);
    }

    ~mycont() {
        allocator.deallocate(_mem, my_size);
    }

    mycont(const mycont &) = delete;
    mycont(const mycont &&) = delete;
    mycont &operator =(const mycont &) = delete;
    mycont &operator =(const mycont &&) = delete;

    void append(T &elem) {
        if (_count == my_size) {
            throw std::bad_alloc();
        }
        _mem[_count ++] = elem;
    }

    size_t size() const {
        return _count;
    }

    bool empty() const {
        return _count == 0;
    }

    iterator begin() {
        return _mem;
    }

    iterator end() {
        return _mem + _count;
    }

    T& operator[](size_t pos) {
        return _mem[pos];
    }

    T& at(size_t pos) {
        return _mem[pos];
    }

private:
    Allocator allocator;
    T* _mem;
    size_t _count = 0;
};


#endif //MYALLOC_MYCONT_H

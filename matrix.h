#ifndef __MATRIX_H_LOADED___
#define __MATRIX_H_LOADED___

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <array>
#include <vector>
#include <unordered_map>
#include <exception>
#include <stdexcept>

class partial_comparison_error : public std::runtime_error
{
public:
    partial_comparison_error() : std::runtime_error("no enough indexes"){}
};

template <typename T, T default_value, int N=2>
class Matrix {
public:
    using key_t = std::array<int, N>;

    struct key_hash
    {
        std::size_t operator()(const key_t& k) const
        {
            auto h = std::hash<int>();
            std::size_t hash = 0;
            for (auto v: k) {
                hash ^= h(v);
            }
            return hash;
        }
    };

	std::size_t size() {
        return _busy_cells.size();
    }

    struct matrix_idx {

        matrix_idx(Matrix &base) : _matrix(base) {}

        matrix_idx &operator[](int col) {
            _index[pos++] = col;
            return *this;
        }

        operator T() {
            return _matrix.get(_index);
        }

        bool operator ==(T other) {
            if (_index.size() != N) {
                throw partial_comparison_error();
            }

            T val = _matrix.get(_index);
            return val == other;
        }

        matrix_idx &operator = (T val) {
            _matrix.set(_index, val);
            return *this;
        }

        void _append(int row) {
            _index[pos++] = row;
        }

        key_t _index;
        size_t pos = 0;
        Matrix<T, default_value, N> &_matrix;
    };

    matrix_idx operator[](int row) {
        matrix_idx ret(*this);
        ret._append(row);
        return ret;
    }

    struct iterator {
        bool operator != (const iterator &other) {
            return _it != other._it;
        }

        void operator++() {
            _it++;
        }

        // convert array into a tuple
        template<typename Array, std::size_t... I>
        auto a2t_impl(const Array& a, std::index_sequence<I...>, T val)
        {
            return std::make_tuple(a[I]..., val);
        }

        template<typename Indices = std::make_index_sequence<N>>
        auto a2t(const key_t & a, T val)
        {
            return a2t_impl(a, Indices{}, val);
        }

        auto operator *() {
            return a2t(_it->first, _it->second);
        }

        typename std::unordered_map<key_t, T, key_hash>::iterator _it;
    };

	iterator begin() {
        auto it = _busy_cells.begin();
        return iterator{it};
    }

	iterator end() {
        auto it = _busy_cells.end();
        return iterator{it};
    }

    T get(const key_t &idx) {
        auto f = _busy_cells.find(idx);
        if (f == _busy_cells.end()) {
            return default_value;
        } else {
            return f->second;
        }
    }

    void set(const key_t &idx, T val) {
        auto f = _busy_cells.find(idx);
        if (f == _busy_cells.end()) {
            if (val != default_value) {
                _busy_cells[idx] = val;
            } // else do nothing, no need to insert default value
        } else {
            if (val == default_value) {
                // remove value equal to default
                _busy_cells.erase(idx);
            } else {
                // override existing value
                _busy_cells[idx] = val;
            }
        }
    }

private:
    std::unordered_map<key_t, T, key_hash> _busy_cells;

};


#endif
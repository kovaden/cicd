# Разреженная матрица

This is a header-only implementation of N-dimentional sparse matrix. Sparse matrix stores only cells which are actually used, and they value is different from default.

Usage:

    #include "matrix.h"

    Matrix<int, -1> matrix;
    matrix[1][2] = 5

Build sample:
    mkdir build && cd build && cmake .. && cmake --build .

Run sample:
    ./matrix


template <typename T, T default_value, int N=2>
class Matrix

T - values type 
default_value - default values for the matrix, which is not stored
N - number of array dimensions

The Matrix overloads [] operator for any number on dimensions so it is possible to get and access variables in form:

    matrix[1][2][3] = 5
    m = matrix[1][2][3]


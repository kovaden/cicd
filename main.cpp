#include "matrix.h"

#include <iostream>
#include <iomanip>
#include <cassert>

#include <tuple>

using namespace std;


void matrix_sample1() {// бесконечная матрица int заполнена значениями -1
    Matrix<int, -1> matrix;
    assert(matrix.size() == 0); // все ячейки свободны

    auto a = matrix[0][0];

    assert(a == -1);
    assert(matrix.size() == 0);

    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);

    // выведется одна строка
// 100100314
    for(auto c: matrix)
    {
        int x;
        int y;
        int v;
        tie(x, y, v) = c;
        cout << x << y << v << endl;
    }

    cout << a << endl;

    ((matrix[100][100] = 314) = 0) = 217;

}

void matrix_sample2()
{
    const auto size = 10;
    Matrix<int, 0> matrix;

    for (auto i = 0; i < size; i++) {
        matrix[i][i] = i;
        matrix[i][size - i - 1] = size - i - 1;
    }

    for (auto x = 1; x < size-1; x++) {
        for (auto y = 1; y < size-1; y++) {
            cout << matrix[x][y] << " ";
        }
        cout << endl;
    }

    cout << "Occupied cells: " << matrix.size() << endl;

    for (auto it : matrix) {
        cout << "[" << get<0>(it) << "," << get<1>(it) << "] = " << get<2>(it) << endl;
    }

    assert(matrix.size() == 18);
}

void sample_cubic() {

    cout << "Sample with 3D matrix:" << endl;

    Matrix<int, -1, 3> matrix3d;
    auto a = matrix3d[1][1][1];

    assert(a == -1);

    matrix3d[1][2][3] = 3;

    const int size = 4;

    for (auto x = 0; x < size; x++) {
        for (auto y = 0; y < size; y++) {
            for (auto z = 0; z < size; z++) {
                cout << setw(3) << matrix3d[x][y][z] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    cout << endl;

    assert(matrix3d.size() == 1);
}

int main (int, char **) {
    matrix_sample1();

    matrix_sample2();

    sample_cubic();

    return 0;
}

#ifndef UTIL_H_
#define UTIL_H_

#include <iomanip>
#include <iostream>
using namespace std;

template <typename T> void print_vector(T *vector, int n) {
  for (int i = 0; i < n; i++) {
    cout << setprecision(5) << *vector << "\t";
    vector++;
  }
  cout << endl;
}

template <typename T> void print_matrix(T *matrix, int m, int n) {
  for (int i = 0; i < m; i++) {
    cout << "Row " << i << ": [ ";
    for (int j = 0; j < n; j++) {
      cout << setprecision(5) << matrix[i * n + j] << "\t";
    }
    cout << " ]" << endl;
  }
}

template <typename T> void print_matrix(T *matrix, int m) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      cout << setprecision(5) << matrix[i * m + j] << "\t";
    }
    cout << endl;
  }
}
template <typename T> void zero_matrix_init(T *matrix, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i * n + j] = 0;
    }
  }
}

#endif // !UTIL_H_

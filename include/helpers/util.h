#ifndef UTIL_H_
#define UTIL_H_

#include <iomanip>
#include <iostream>
using namespace std;

template <typename T> void print_vector(T *vector, int n) {
  for (int i = 0; i < n; i++) {
    cout << setprecision(3) << *vector << "\t";
    vector++;
  }
  cout << endl;
}

template <typename T> void print_matrix(T *matrix, int m, int n) {
  for (int i = 0; i < m; i++) {
    cout << "Row " << i << ": [ ";
    for (int j = 0; j < n; j++) {
      cout << setprecision(3) << matrix[i * n + j] << "\t";
    }
    cout << " ]" << endl;
  }
}

template <typename T> void print_matrix(T *matrix, int m) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      cout << setprecision(3) << matrix[i * m + j] << "\t";
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

template <typename T>
void matrix_multiplication(T *result, T *matrix, T *matrix0, int dim) {
  for (int i = 0; i < dim; ++i) {
    for (int j = 0; j < dim; ++j) {
      for (int k = 0; k < dim; ++k) {
        result[i * dim + j] += matrix[i * dim + k] * matrix0[k * dim + j];
      }
    }
  }
}

template <typename T>
void matrix_multiplication(T *result, T *matrix, T *matrix0, int m, int n,
                           int p) {
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < p; ++j) {
      for (int k = 0; k < n; ++k) {
        result[i * p + j] += matrix[i * n + k] * matrix0[k * p + j];
      }
    }
  }
}

#endif // !UTIL_H_

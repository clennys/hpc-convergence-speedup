#ifndef SPD_MATRIX
#define SPD_MATRIX

#include <cstdlib>
#include <iostream>
using namespace std;

template <typename T>
T generate_random_value(int offset, int range, bool seed) {
  if (seed) {
    srand((unsigned)time(NULL));
  }
  return offset + (rand() % range);
}

template <typename T> void transpose_matrix(T *matrix, T *transpose, int dim) {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      transpose[i * dim + j] = matrix[j * dim + i];
    }
  }
}

template <typename T>
void matrix_multiplication(T *result, T *matrix, T *transpose, int dim) {
  for (int i = 0; i < dim; ++i) {
    for (int j = 0; j < dim; ++j) {
      result[i * dim + j] = 0;
    }
  }
  for (int i = 0; i < dim; ++i) {
    for (int j = 0; j < dim; ++j) {
      for (int k = 0; k < dim; ++k) {
        result[i * dim + j] += matrix[i * dim + k] * transpose[k * dim + j];
      }
    }
  }
}

template <typename T>
void matrix_multiplication(T *result, T *matrix, T *matrix0, int m, int n,
                           int p) {
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < p; ++j) {
      result[i * p + j] = 0;
    }
  }
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      for (int k = 0; k < p; ++k) {
        result[i * n + j] += matrix[i * p + k] * matrix0[k * p + j];
      }
    }
  }
}

template <typename T> void spd_generator(T *result, int dim) {
  double *matrix = new double[dim * dim];
  double *transpose = new double[dim * dim];
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      if (i == j) {
        matrix[i * dim + j] = generate_random_value<T>(100, 10, false);
      } else if (i > j) {
        matrix[i * dim + j] = generate_random_value<T>(0, 10, false);
      } else {
        matrix[i * dim + j] = 0;
      }
    }
  }
  transpose_matrix<T>(matrix, transpose, dim);
  matrix_multiplication<T>(result, matrix, transpose, dim);
}

/* Code for further processing and free the
  dynamically allocated memory */
template <typename T> void free_spd_matrix(T *matrix) { free(matrix); }

#endif // !SPD_MATRIX

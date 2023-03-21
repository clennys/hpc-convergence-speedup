#ifndef SPD_MATRIX_H_
#define SPD_MATRIX_H_

#include "../helpers/util.h"
#include <cstdlib>
#include <iostream>
using namespace std;

template <typename T> T generate_random_value(int offset, int range) {
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
void matrix_multiplication(T *result, T *matrix, T *matrix0, int dim) {
  zero_matrix_init(result, dim, dim);

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
  zero_matrix_init(result, m, p);
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < p; ++j) {
      for (int k = 0; k < n; ++k) {
        result[i * p + j] += matrix[i * n + k] * matrix0[k * p + j];
      }
    }
  }
}

template <typename T> void spd_matrix_generator(T *result, int dim, bool rd) {
  double *matrix = new double[dim * dim];
  double *transpose = new double[dim * dim];
  if (rd) {
    srand((unsigned)time(NULL));
  }

  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      if (i == j) {
        matrix[i * dim + j] = generate_random_value<T>(100, 10);
      } else if (i > j) {
        matrix[i * dim + j] = generate_random_value<T>(0, 10);
      } else {
        matrix[i * dim + j] = 0;
      }
    }
  }
  transpose_matrix<T>(matrix, transpose, dim);
  matrix_multiplication<T>(result, matrix, transpose, dim);
}

template <typename T> void vector_x_generator(T *vector, int dim, bool rd) {
  if (rd) {
    srand((unsigned)time(NULL));
  }

  for (int i = 0; i < dim; i++) {

    vector[i] = generate_random_value<T>(0, 10);
  }
}

/* Code for further processing and free the
  dynamically allocated memory */
template <typename T> void free_spd_matrix(T *matrix) { free(matrix); }

#endif // !SPD_MATRIX_H_

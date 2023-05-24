#include "../../../include/iterative_solvers/methods/damped_jacobi.h"
#include "../../../include/helpers/mpi_module.h"
#include "../../../include/helpers/util.h"
#include "../../../include/inputs/param.h"
#include "../../../include/iterative_solvers/methods/gauss_seidel.h"
#include "mpi.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <math.h>

using namespace std;

namespace damped_jacobi {
namespace parallel {

void setup(double *mat, double *x_init, double *b, double *sub_mat, param p) {

  MPI_Bcast(b, p.matrix_dim, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
  MPI_Bcast(x_init, p.matrix_dim, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);

  MPI_Scatter(mat, p.block_length * p.matrix_dim, MPI_DOUBLE, sub_mat,
              p.block_length * p.matrix_dim, MPI_DOUBLE, ROOT_PROC,
              MPI_COMM_WORLD);
}

void calc_xnew(double *sub_mat, double *x, double *b, double *x_new,
               double *x_norm, param p) {
  double sum;
  *x_norm = 0;

  for (int i = 0; i < p.rank_block_length; i++) {
    sum = 0;
    for (int j = 0; j < p.matrix_dim_no_empty_rows; j++) {
      sum += sub_mat[i * p.matrix_dim + j] * x[j];
    }

    x_new[i] = x[p.my_rank * p.block_length + i] -
               (p.omega /
                sub_mat[i * p.matrix_dim + (p.my_rank * p.block_length + i)]) *
                   (sum - b[p.my_rank * p.block_length + i]);
    *x_norm += x_new[i] * x_new[i];
  }
}
void calc_r(double *sub_mat, double *x, double *b, double *r_local,
            double *r_norm, param p) {
  gauss_seidel::parallel::calc_r(sub_mat, x, b, r_local, r_norm, p);
}

void step(double *sub_mat, double *x, double *b, double *x_new, double *x_norm,
          param p) {

  calc_xnew(sub_mat, x, b, x_new, x_norm, p);

  MPI_Allgather(x_new, p.block_length, MPI_DOUBLE, x, p.block_length,
                MPI_DOUBLE, MPI_COMM_WORLD);
}

bool stopping_criterion(double *r_norm, double *x_norm, param p) {
  return gauss_seidel::parallel::stopping_criterion(r_norm, x_norm, p);
}

int run(double *mat, double *x, double *x_new, double *b, param p) {
  double sub_mat[p.block_length * p.matrix_dim];
  double r_local[p.block_length];
  double r_norm = 0;
  double x_norm = 0;

  setup(mat, x, b, sub_mat, p);

  int counter = 0;
  while (true) {
    step(sub_mat, x, b, x_new, &x_norm, p);
    calc_r(sub_mat, x_new, b, r_local, &r_norm, p);

    counter++;
    if (stopping_criterion(&r_norm, &x_norm, p)) {
      return counter;
    }
  }
  return -1;
}
} // namespace parallel

namespace serial {
void step(double *x_new, double *matrix, double *b, double *x, param p) {
  double sum;
  for (int i = 0; i < p.matrix_dim; i++) {
    sum = 0;
    for (int j = 0; j < p.matrix_dim; j++) {
      sum += matrix[i * p.matrix_dim + j] * x[j];
    }
    x_new[i] = x[i] - (p.omega / matrix[i * p.matrix_dim + i] * (sum - b[i]));
  }
}

void calc_r(double *matrix, double *x, double *b, double *r, param p) {
  double sum;
  for (int i = 0; i < p.matrix_dim; i++) {
    sum = 0;
    for (int j = 0; j < p.matrix_dim; j++) {
      sum += matrix[i * p.matrix_dim + j] * x[j];
    }
    r[i] = sum - b[i];
  }
}

bool stopping_criterion(double *matrix, double *x, double *b, double *r,
                        param p) {
  calc_r(matrix, x, b, r, p);

  double norm_x = 0;
  double norm_r = 0;

  for (int i = 0; i < p.matrix_dim; i++) {
    norm_x += x[i] * x[i];
    norm_r += r[i] * r[i];
  }
  double diff = sqrt(norm_r) / sqrt(norm_x);
  diff = diff > 0 ? diff : -diff;
  // cout << "diff: " << diff << " epsilon: " << p.epsilon << endl;
  if (diff < p.epsilon) {
    return false;
  }
  return true;
}
int run(double *matrix, double *x, double *x_new, double *b, param p) {
  double *r = new double[p.matrix_dim];
  int counter = 0;
  do {
    step(x_new, matrix, b, x, p);
    double *tmp;
    tmp = x_new;
    x_new = x;
    x = tmp;
    counter++;
  } while (stopping_criterion(matrix, x, b, r, p));
  return counter;
};
}; // namespace serial

} // namespace damped_jacobi

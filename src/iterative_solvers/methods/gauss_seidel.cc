#include "../../../include/iterative_solvers/methods/gauss_seidel.h"
#include "../../../include/helpers/mpi_module.h"
#include "../../../include/helpers/util.h"
#include "../../../include/inputs/param.h"
#include "mpi.h"
#include <iostream>
#include <math.h>
namespace gauss_seidel {
namespace parallel {

void setup(double *mat, double *sub_mat, double *x_init, double *x_new,
           double *b, param p) {
  MPI_Bcast(b, p.matrix_dim, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
  MPI_Bcast(x_init, p.matrix_dim, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);

  MPI_Scatter(mat, p.block_length * p.matrix_dim, MPI_DOUBLE, sub_mat,
              p.block_length * p.matrix_dim, MPI_DOUBLE, ROOT_PROC,
              MPI_COMM_WORLD);
}

void calc_r(double *sub_mat, double *x, double *b, double *r_local,
            double *r_norm, param p) {
  *r_norm = 0;
  double sum;
  for (int i = 0; i < p.rank_block_length; i++) {
    sum = 0;
    for (int j = 0; j < p.matrix_dim_no_empty_rows; j++) {
      sum += sub_mat[i * p.matrix_dim + j] * x[j];
    }
    r_local[i] = sum - b[p.my_rank * p.block_length + i];
    *r_norm += r_local[i] * r_local[i];
  }
}

void forward_substitution(double *mat_GS, double *r, double *y, param p) {

  // zeros_matrix(y, mat_dim, 1);
  y[0] = r[0] / mat_GS[0];
  double sum;
  for (int i = 1; i < p.matrix_dim_no_empty_rows; i++) {
    sum = 0;
    for (int j = 0; j < i; j++) {
      sum += mat_GS[i * p.matrix_dim + j] * y[j];
    }
    y[i] = (r[i] - sum) / mat_GS[i * p.matrix_dim + i];
  }
}

void step(double *mat, double *sub_mat, double *x, double *b, double *r_local,
          double *r_gathered, double *y_scatter, double *y_local, double *x_new,
          double *r_norm, double *x_norm, param p) {

  calc_r(sub_mat, x, b, r_local, r_norm, p);

  MPI_Gather(r_local, p.block_length, MPI_DOUBLE, r_gathered, p.block_length,
             MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);

  if (p.my_rank == ROOT_PROC) {
    forward_substitution(mat, r_gathered, y_scatter, p);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Scatter(y_scatter, p.block_length, MPI_DOUBLE, y_local, p.block_length,
              MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
  *x_norm = 0;
  for (int i = 0; i < p.rank_block_length; i++) {
    x_new[i] = x[p.my_rank * p.block_length + i] - y_local[i];
    *x_norm += x_new[i] * x_new[i];
  }

  MPI_Allgather(x_new, p.block_length, MPI_DOUBLE, x, p.block_length,
                MPI_DOUBLE, MPI_COMM_WORLD);
}

bool stopping_criterion(double *r_norm, double *x_norm, param p) {

  double norm_r, norm_x;

  MPI_Allreduce(r_norm, &norm_r, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(x_norm, &norm_x, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  double diff = sqrt(norm_r) / sqrt(norm_x);
  diff = diff > 0 ? diff : -diff;
  if (p.my_rank == ROOT_PROC) {
    //   cout << "r norm: " << norm_r << " x norm: " << norm_x << endl;
    cout << "diff: " << diff << " epsilon: " << p.epsilon << endl;
  }
  return diff < p.epsilon ? true : false;
}

int run(double *mat, double *x, double *x_new, double *b, param p) {

  double sub_mat[p.block_length * p.matrix_dim], r_local[p.block_length],
      r_gathered[p.matrix_dim], y_scatter[p.matrix_dim],
      y_local[p.block_length];
  double r_norm = 0.0;
  double x_norm = 0.0;

  setup(mat, sub_mat, x, x_new, b, p);

  int counter = 0;
  while (true) {
    step(mat, sub_mat, x, b, r_local, r_gathered, y_scatter, y_local, x_new,
         &r_norm, &x_norm, p);
    counter++;
    if (stopping_criterion(&r_norm, &x_norm, p)) {
      return counter;
    }
  }
  return -1;
}
} // namespace parallel
namespace serial {
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

void step(double *x_new, double *matrix, double *b, double *x, double *r,
          double *y, param p) {

  calc_r(matrix, x, b, r, p);

  gauss_seidel::parallel::forward_substitution(matrix, r, y, p);

  for (int i = 0; i < p.block_length; i++) {
    x_new[i] = x[i] - y[i];
  }
}

bool stopping_criterion(double *matrix, double *x, double *b, double *r,
                        param p) {

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
  double *y = new double[p.matrix_dim];
  int counter = 0;
  do {
    step(x_new, matrix, b, x, r, y, p);
    double *tmp;
    tmp = x_new;
    x_new = x;
    x = tmp;
    counter++;
  } while (stopping_criterion(matrix, x, b, r, p));
  return counter;
}
} // namespace serial

} // namespace gauss_seidel

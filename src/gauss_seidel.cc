#include "../include/gauss_seidel.h"
#include "../include/mpi_module.h"
#include "../include/param.h"
#include "../include/util.h"
#include "mpi.h"
#include <iostream>
#include <math.h>

void gauss_seidel_setup(double *mat, double *sub_mat, double *x_init,
                        double *x_new, double *b, param p) {
  MPI_Bcast(b, p.mat_dim, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
  MPI_Bcast(x_init, p.mat_dim, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);

  MPI_Scatter(mat, p.block_length * p.mat_dim, MPI_DOUBLE, sub_mat,
              p.block_length * p.mat_dim, MPI_DOUBLE, ROOT_PROC,
              MPI_COMM_WORLD);
}

void gauss_seidel_r(double *sub_mat, double *x, double *b, double *r_local,
                    param p) {
  double sum;
  for (int i = 0; i < p.block_length; i++) {
    sum = 0;
    for (int j = 0; j < p.mat_dim; j++) {
      sum += sub_mat[i * p.mat_dim + j] * x[j];
    }
    r_local[i] = sum - b[p.my_rank * p.block_length + i];
  }
}

void gauss_seidel_forward_substitution(double *mat_GS, double *r, double *y,
                                       int mat_dim) {

  // zeros_matrix(y, mat_dim, 1);
  y[0] = r[0] / mat_GS[0];
  double sum;
  for (int i = 1; i < mat_dim; i++) {
    sum = 0;
    for (int j = 0; j < i; j++) {
      sum += mat_GS[i * mat_dim + j] * y[j];
    }
    y[i] = (r[i] - sum) / mat_GS[i * mat_dim + i];
  }
}

void gauss_seidel_step(double *mat, double *sub_mat, double *x, double *b,
                       double *r_local, double *r_gathered, double *y_scatter,
                       double *y_local, double *x_new, param p) {

  gauss_seidel_r(sub_mat, x, b, r_local, p);

  MPI_Gather(r_local, p.block_length, MPI_DOUBLE, r_gathered, p.block_length,
             MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);

  if (p.my_rank == ROOT_PROC) {
    gauss_seidel_forward_substitution(mat, r_gathered, y_scatter, p.mat_dim);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Scatter(y_scatter, p.block_length, MPI_DOUBLE, y_local, p.block_length,
              MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
  for (int i = 0; i < p.block_length; i++) {
    x_new[i] = x[p.my_rank * p.block_length + i] - y_local[i];
  }

  MPI_Allgather(x_new, p.block_length, MPI_DOUBLE, x, p.block_length,
                MPI_DOUBLE, MPI_COMM_WORLD);
}

bool gauss_seidel_terminate(double *r, double *x, param p) {
  double norm_r = 0;
  double norm_x = 0;

  for (int i = 0; i < p.mat_dim; i++) {
    norm_x += x[i] * x[i];
    norm_r += r[i] * r[i];
  }
  double diff = sqrt(norm_r) / sqrt(norm_x);
  diff = diff > 0 ? diff : -diff;
  cout << "DIFF: " << diff << endl;
  cout << "R: " << endl;
  print_vector(r, p.mat_dim);
  cout << "X: " << endl;
  print_vector(x, p.mat_dim);

  if (diff < p.epsilon) {
    return true;
  }
  return false;
}

void gauss_seidel(double *mat, double *x, double *b, param p) {

  double x_new[p.block_length], sub_mat[p.block_length * p.mat_dim],
      r_local[p.block_length], r_gathered[p.mat_dim], y_scatter[p.mat_dim],
      y_local[p.block_length];

  gauss_seidel_setup(mat, sub_mat, x, x_new, b, p);

  int count = 0;
  int continues = 1;
  while (true) {
    gauss_seidel_step(mat, sub_mat, x, b, r_local, r_gathered, y_scatter,
                      y_local, x_new, p);
    if (p.my_rank == ROOT_PROC) {
      if (gauss_seidel_terminate(r_gathered, x, p)) {
        continues = 0;
      }
    }

    MPI_Bcast(&continues, 1, MPI_INT, ROOT_PROC, MPI_COMM_WORLD);

    if (continues == 0) {
      return;
    }

    count++;
  }
}

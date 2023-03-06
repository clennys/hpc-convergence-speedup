#include "../include/gauss_seidel.h"
#include "../include/mpi_module.h"
#include "../include/util.h"
#include "mpi.h"
#include <iostream>

void gauss_seidel_setup(double *mat, double *sub_mat, double *x_init,
                        double *x_new, double *b, int block_length,
                        int mat_dim) {
  MPI_Bcast(b, mat_dim, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
  MPI_Bcast(x_init, mat_dim, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);

  MPI_Scatter(mat, block_length * mat_dim, MPI_DOUBLE, sub_mat,
              block_length * mat_dim, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
}

void gauss_seidel_r(double *sub_mat, double *x, double *b, double *r_local,
                    int block_length, int mat_dim, int my_rank) {
  double sum;
  for (int i = 0; i < block_length; i++) {
    sum = 0;
    for (int j = 0; j < mat_dim; j++) {
      sum += sub_mat[i * mat_dim + j] * x[j];
    }
    r_local[i] = sum - b[my_rank * block_length + i];
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
                       double *y_local, double *x_new, int block_length,
                       int mat_dim, int my_rank) {

  gauss_seidel_r(sub_mat, x, b, r_local, block_length, mat_dim, my_rank);

  MPI_Gather(r_local, block_length, MPI_DOUBLE, r_gathered, block_length,
             MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);

  if (my_rank == ROOT_PROC) {
    gauss_seidel_forward_substitution(mat, r_gathered, y_scatter, mat_dim);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Scatter(y_scatter, block_length, MPI_DOUBLE, y_local, block_length,
              MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
  for (int i = 0; i < block_length; i++) {
    x_new[i] = x[my_rank * block_length + i] - y_local[i];
  }

  MPI_Allgather(x_new, block_length, MPI_DOUBLE, x, block_length, MPI_DOUBLE,
                MPI_COMM_WORLD);
}

void gauss_seidel(double *mat, double *x, double *b, int mat_dim, int nr_proc,
                  int my_rank) {
  int block_length = mat_dim / nr_proc;
  double x_new[block_length], sub_mat[block_length * mat_dim],
      r_local[block_length], r_gathered[mat_dim], y_scatter[mat_dim],
      y_local[block_length];

  gauss_seidel_setup(mat, sub_mat, x, x_new, b, block_length, mat_dim);

  int count = 0;
  while (count < 4) {
    gauss_seidel_step(mat, sub_mat, x, b, r_local, r_gathered, y_scatter,
                      y_local, x_new, block_length, mat_dim, my_rank);
    count++;
  }
}

#include "../include/mpi_module.h"
#include "../include/spd_matrix_generator.h"
#include "../include/util.h"
#include "mpi.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

void damped_jacobi_setup(double *mat, double *x_new, double *b, double *sub_mat,
                         int mat_dim, int block_length, int my_rank) {
  zeros_matrix(x_new, block_length, 1);

  // Broadcast omega from Root to all other processes
  // MPI_Bcast(&omega, 1, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
  MPI_Bcast(b, mat_dim, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);

  MPI_Scatter(mat, block_length * mat_dim, MPI_DOUBLE, sub_mat,
              block_length * mat_dim, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
}

void jacobi_step(double *sub_mat, double *x, double *b, double *x_new,
                 int block_length, int mat_dim, int omega, int my_rank) {
  double sum;

  for (int i = 0; i < block_length; i++) {
    sum = 0;
    for (int j = 0; j < mat_dim; j++) {
      sum += sub_mat[i * mat_dim + j] * x[j];
    }

    x_new[i] = x[my_rank * block_length + i] -
               (omega / sub_mat[i * mat_dim + (my_rank * block_length + i)]) *
                   (sum - b[my_rank * block_length + i]);
  }

  MPI_Allgather(x_new, block_length, MPI_DOUBLE, x, block_length, MPI_DOUBLE,
                MPI_COMM_WORLD);
}

int damped_jacobi(double *mat, double *x, double *b, int omega, int mat_dim,
                  int my_rank, int nr_proc) {
  int block_length = mat_dim / nr_proc;
  double x_new[block_length];
  double sub_mat[block_length * mat_dim];

  damped_jacobi_setup(mat, x_new, b, sub_mat, mat_dim, block_length, my_rank);

  int count = 0;
  while (count < 10) {
    jacobi_step(sub_mat, x, b, x_new, block_length, mat_dim, omega, my_rank);
    count++;
  }

  return 0;
}

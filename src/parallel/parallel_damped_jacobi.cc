#include "../../include/helpers/util.h"
#include "../../include/inputs/param.h"
#include "../../include/inputs/spd_matrix_generator.h"
#include "../../include/parallel/mpi_module.h"
#include "../../include/parallel/parallel_gauss_seidel.h"
#include "mpi.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

void damped_jacobi_setup(double *mat, double *x_init, double *b,
                         double *sub_mat, param p) {

  MPI_Bcast(b, p.mat_dim, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
  MPI_Bcast(x_init, p.mat_dim, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);

  MPI_Scatter(mat, p.block_length * p.mat_dim, MPI_DOUBLE, sub_mat,
              p.block_length * p.mat_dim, MPI_DOUBLE, ROOT_PROC,
              MPI_COMM_WORLD);
}

void damped_jacobi_xnew(double *sub_mat, double *x, double *b, double *x_new,
                        param p) {
  double sum;

  for (int i = 0; i < p.block_length; i++) {
    sum = 0;
    for (int j = 0; j < p.mat_dim; j++) {
      sum += sub_mat[i * p.mat_dim + j] * x[j];
    }

    x_new[i] =
        x[p.my_rank * p.block_length + i] -
        (p.omega / sub_mat[i * p.mat_dim + (p.my_rank * p.block_length + i)]) *
            (sum - b[p.my_rank * p.block_length + i]);
  }
}
void damped_jacobi_r(double *sub_mat, double *x, double *b, double *r_local,
                     double *r_gathered, param p) {
  gauss_seidel_r(sub_mat, x, b, r_local, p);
  MPI_Gather(r_local, p.block_length, MPI_DOUBLE, r_gathered, p.block_length,
             MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
}

void damped_jacobi_step(double *sub_mat, double *x, double *b, double *x_new,
                        param p) {

  damped_jacobi_xnew(sub_mat, x, b, x_new, p);

  MPI_Allgather(x_new, p.block_length, MPI_DOUBLE, x, p.block_length,
                MPI_DOUBLE, MPI_COMM_WORLD);
}

bool damped_jacobi_terminate(double *r, double *x, param p) {
  return gauss_seidel_terminate(r, x, p);
}

void damped_jacobi(double *mat, double *x, double *b, param p) {
  double x_new[p.block_length];
  double sub_mat[p.block_length * p.mat_dim];
  double r_local[p.block_length], r_gathered[p.mat_dim];

  damped_jacobi_setup(mat, x, b, sub_mat, p);

  int count = 0;
  int continues = 1;
  while (true) {
    damped_jacobi_step(sub_mat, x, b, x_new, p);
    damped_jacobi_r(sub_mat, x, b, r_local, r_gathered, p);

    if (p.my_rank == ROOT_PROC) {
      if (damped_jacobi_terminate(r_gathered, x, p)) {
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

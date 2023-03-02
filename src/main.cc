#include "../include/damped_jacobi.h"
#include "../include/mpi_module.h"
#include "../include/spd_matrix_generator.h"
#include "../include/util.h"
#include "mpi.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  int size, my_rank;

  startup_MPI(&my_rank, &size);
  double *b;
  double *matrix;
  double *x;
  int omega = 1;
  int mat_dim = 10;

  b = new double[mat_dim];
  x = new double[mat_dim];
  zeros_matrix(x, mat_dim, 1);
  if (my_rank == ROOT_PROC) {
    b = new double[mat_dim]{34917, 45797, 58677,  59040,  74866,
                            81937, 95400, 124676, 119564, 18196};
    matrix = new double[mat_dim * mat_dim];
    x = new double[mat_dim];
    spd_generator(matrix, mat_dim);
    print_matrix(matrix, mat_dim);
  }

  damped_jacobi(matrix, x, b, omega, mat_dim, my_rank, size);

  if (ROOT_PROC == my_rank) {
    cout << endl << "Solution for x is: " << endl;
    print_vector(x, mat_dim);
  }

  finish_MPI();
  return 0;
}

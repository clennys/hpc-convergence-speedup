#include "../include/damped_jacobi.h"
#include "../include/gauss_seidel.h"
#include "../include/mpi_module.h"
#include "../include/param.h"
#include "../include/spd_matrix_generator.h"
#include "../include/util.h"
#include "mpi.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  int size, my_rank;

  startup_MPI(&my_rank, &size);
  double *b;
  double *matrix;
  double *x, *x_true;

  param p(my_rank, size);
  p.read_param_from_file("input.txt");

  b = new double[p.mat_dim];
  x = new double[p.mat_dim];
  if (my_rank == ROOT_PROC) {
    zero_matrix_init(x, p.mat_dim, 1);

    matrix = new double[p.mat_dim * p.mat_dim];
    spd_matrix_generator(matrix, p.mat_dim, true);
    x_true = new double[p.mat_dim];
    vector_x_generator(x_true, p.mat_dim, true);
    matrix_multiplication(b, matrix, x_true, p.mat_dim, p.mat_dim, 1);
    // print_matrix(matrix, p.mat_dim);
  }

  double start = MPI_Wtime();
  if (argc < 3) {
    if (strcmp(argv[1], "-dj") == 0) {
      if (my_rank == ROOT_PROC) {
        cout << "===== Executing Damped Jacobi =====" << endl;
      }
      damped_jacobi(matrix, x, b, p);
    } else if (strcmp(argv[1], "-gs") == 0) {
      if (my_rank == ROOT_PROC) {
        cout << "===== Executing Gauss-Seidel  =====" << endl;
      }
      gauss_seidel(matrix, x, b, p);
    } else {
      if (my_rank == ROOT_PROC) {
        cout << "Select Method!" << endl;
      }
    }
  }
  double end = MPI_Wtime();

  if (ROOT_PROC == my_rank) {
    cout << endl << "Solution for x is: " << endl;
    print_vector(x, p.mat_dim);
    cout << endl << "Calc solution for x is: " << endl;
    print_vector(x_true, p.mat_dim);
    cout << endl << "b is: " << endl;
    print_vector(b, p.mat_dim);
    cout << "The process took " << end - start << " seconds to run." << endl;
  }

  finish_MPI();
  return 0;
}

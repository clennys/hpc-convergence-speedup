#include "../include/helpers/mpi_module.h"
#include "../include/helpers/util.h"
#include "../include/inputs/linear_system_generator.h"
#include "../include/inputs/param.h"
#include "../include/inputs/spd_matrix_generator.h"
#include "../include/iterative_solvers/damped_jacobi.h"
#include "../include/iterative_solvers/gauss_seidel.h"
#include "mpi.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  int size, my_rank;

  startup_MPI(&my_rank, &size);
  double *b, *b_check;
  double *matrix;
  double *x; //*x_true;

  param p(my_rank, size);
  p.read_param_from_file("input.txt");

  b = new double[p.matrix_dim];
  x = new double[p.matrix_dim];
  if (my_rank == ROOT_PROC) {
    zero_matrix_init(x, p.matrix_dim, 1);

    matrix = new double[p.matrix_dim * p.matrix_dim];
    // spd_matrix_generator(matrix, p.matrix_dim, true);
    // x_true = new double[p.matrix_dim];
    // vector_x_generator(x_true, p.matrix_dim, true);
    // matrix_multiplication(b_check, matrix, x_true, p.matrix_dim,
    // p.matrix_dim, 1);
    discretized_grid::five_point_stencil(matrix, b, p);
    print_matrix(matrix, p.matrix_dim);
  }

  double start = MPI_Wtime();
  if (argc < 3) {
    if (strcmp(argv[1], "-dj") == 0) {
      if (my_rank == ROOT_PROC) {
        cout << "===== Executing Damped Jacobi =====" << endl;
      }
      damped_jacobi::parallel::run(matrix, x, b, p);
    } else if (strcmp(argv[1], "-gs") == 0) {
      if (my_rank == ROOT_PROC) {
        cout << "===== Executing Gauss-Seidel  =====" << endl;
      }
      gauss_seidel::parallel::run(matrix, x, b, p);
    } else {
      if (my_rank == ROOT_PROC) {
        cout << "Select Method!" << endl;
      }
    }
  }
  double end = MPI_Wtime();

  if (ROOT_PROC == my_rank) {
    cout << endl << "Solution for x is: " << endl;
    print_vector(x, p.matrix_dim);
    // cout << endl << "Calc solution for x is: " << endl;
    // print_vector(x_true, p.matrix_dim);
    cout << endl << "b is: " << endl;
    print_vector(b, p.matrix_dim);
    cout << "The process took " << end - start << " seconds to run." << endl;

    cout << endl << "b_check is: " << endl;
    b_check = new double[p.matrix_dim];
    matrix_multiplication(b_check, matrix, x, p.matrix_dim, p.matrix_dim, 1);
    print_vector(b_check, p.matrix_dim);
  }

  finish_MPI();
  return 0;
}

#include "../../include/iterative_solvers/serial.h"
#include "../../include/helpers/mpi_module.h"
#include "../../include/helpers/util.h"
#include "../../include/inputs/linear_system_generator.h"
#include "../../include/inputs/param.h"
#include "../../include/inputs/spd_matrix_generator.h"
#include "../../include/iterative_solvers/methods/damped_jacobi.h"
#include "../../include/iterative_solvers/methods/gauss_seidel.h"

#include "mpi.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

void run_serial(char *opts) {
  cout << "===== Serial Computing =====" << endl;
  double *b, *b_check;
  double *matrix;
  double *x; //*x_true;

  param p(0, 1);
  p.read_param_from_file("input.txt");

  b = new double[p.matrix_dim];
  x = new double[p.matrix_dim];
  matrix = new double[p.matrix_dim * p.matrix_dim];
  zero_matrix_init(x, p.matrix_dim, 1);
  discretized_grid::five_point_stencil(matrix, b, p);
  print_matrix(matrix, p.matrix_dim);

  if (strcmp(opts, "-dj") == 0) {
    cout << "===== Executing Damped Jacobi =====" << endl;
    cout << "here";
    damped_jacobi::serial::run(matrix, x, b, p);
  } else if (strcmp(opts, "-gs") == 0) {
    cout << "===== Executing Gauss-Seidel  =====" << endl;
    // gauss_seidel::serial::run(matrix, x, b, p);
  } else {
    cout << "Select Method!" << endl;
  }
  cout << endl << "Solution for x is: " << endl;
  print_vector(x, p.matrix_dim);
  cout << endl << "b is: " << endl;
  print_vector(b, p.matrix_dim);
  // cout << "The process took " << end - start << " seconds to run." << endl;

  cout << endl << "b_check is: " << endl;
  b_check = new double[p.matrix_dim];
  matrix_multiplication(b_check, matrix, x, p.matrix_dim, p.matrix_dim, 1);
  print_vector(b_check, p.matrix_dim);
};

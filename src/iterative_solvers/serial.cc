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
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;
using namespace std::chrono;

void run_serial(char *solver, char *grid) {
  cout << "===== Serial Computing =====" << endl;
  double *b, *b_check;
  double *matrix;
  double *x;

  param p(0, 1);
  p.read_param_from_file("input.txt");

  b = new double[p.matrix_dim];
  x = new double[p.matrix_dim];
  matrix = new double[p.matrix_dim * p.matrix_dim];
  zero_matrix_init(x, p.matrix_dim, 1);

  if (strcmp(grid, "-fps") == 0) {
    cout << "===== Using Five-Point-Stencil =====" << endl;
    discretized_grid::five_point_stencil(matrix, b, p);
  } else if (strcmp(grid, "-nps") == 0) {
    cout << "===== Using Nine-Point-Stencil =====" << endl;
    discretized_grid::nine_point_stencil(matrix, b, p);
  } else {
    cout << "Select Serial Destretize Method!" << endl;
  }

  print_matrix(matrix, p.matrix_dim);

  auto start = high_resolution_clock::now();
  if (strcmp(solver, "-dj") == 0) {
    cout << "===== Executing Damped Jacobi =====" << endl;
    damped_jacobi::serial::run(matrix, x, b, p);
  } else if (strcmp(solver, "-gs") == 0) {
    cout << "===== Executing Gauss-Seidel  =====" << endl;
    gauss_seidel::serial::run(matrix, x, b, p);
  } else {
    cout << "Select Method for serial computation!" << endl;
  }
  auto end = high_resolution_clock::now();

  auto elapsed = duration_cast<nanoseconds>(end - start);

  cout << endl << "Solution for x is: " << endl;
  print_vector(x, p.matrix_dim);
  cout << endl << "b is: " << endl;
  print_vector(b, p.matrix_dim);

  cout << endl << "b_check is: " << endl;
  b_check = new double[p.matrix_dim];
  matrix_multiplication(b_check, matrix, x, p.matrix_dim, p.matrix_dim, 1);
  print_vector(b_check, p.matrix_dim);

  cout << "The process took " << elapsed.count() * 1e-9 << " seconds to run."
       << endl;
};

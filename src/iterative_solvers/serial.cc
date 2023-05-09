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

void run_serial(string solver, string x_point_formula, int grid_dim) {
  cout << "===== Serial Computing =====" << endl;
  double *b, *b_check;
  double *matrix;
  double *x, *x_new;

  param p(0, 1, grid_dim, solver, x_point_formula);
  p.read("input.txt");

  b = new double[p.matrix_dim];
  x = new double[p.matrix_dim];
  x_new = new double[p.matrix_dim];
  matrix = new double[p.matrix_dim * p.matrix_dim];
  zero_matrix_init(x, p.matrix_dim, 1);
  zero_matrix_init(x_new, p.matrix_dim, 1);

  if (x_point_formula == "-fps") {
    cout << "===== Using Five-Point-Stencil =====" << endl;
    discretized_grid::laplace(matrix, b, p);
  } else if (x_point_formula == "-nps") {
    cout << "===== Using Nine-Point-Stencil =====" << endl;
    discretized_grid::L2_projection(matrix, b, p);
  } else {
    cout << "Select Serial Descretize Method!" << endl;
  }

  // print_matrix(matrix, p.matrix_dim);
  // print_vector(b, p.matrix_dim);

  auto start = high_resolution_clock::now();
  if (solver == "-dj") {
    cout << "===== Executing Damped Jacobi =====" << endl;
    p.iterations = damped_jacobi::serial::run(matrix, x, x_new, b, p);
  } else if (solver == "-gs") {
    cout << "===== Executing Gauss-Seidel  =====" << endl;
    p.iterations = gauss_seidel::serial::run(matrix, x, x_new, b, p);
  } else {
    cout << "Select Method for serial computation!" << endl;
  }
  auto end = high_resolution_clock::now();

  auto elapsed = duration_cast<nanoseconds>(end - start);

  cout << endl << "Solution for x is: " << endl;
  print_vector(x, p.matrix_dim);
  cout << endl << "b is: " << endl;
  print_vector(b, p.matrix_dim);

  // cout << endl << "b_check is: " << endl;
  // b_check = new double[p.matrix_dim];
  // matrix_multiplication(b_check, matrix, x, p.matrix_dim, p.matrix_dim, 1);
  // print_vector(b_check, p.matrix_dim);
  //
  p.time = elapsed.count() * 1e-9;

  cout << "The process took " << elapsed.count() * 1e-9 << " seconds to run."
       << endl;
  p.write_append_csv("output.csv");
};

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

void run_parallel(string solver, string x_point_formula, int grid_size) {
  int size, my_rank;

  double *b, *b_check;
  double *matrix;
  double *x, *x_new;

  startup_MPI(&my_rank, &size);

  if (my_rank == ROOT_PROC) {
    cout << "===== Parallel Computing =====" << endl;
  }
  param p(my_rank, size, grid_size, solver, x_point_formula);
  p.read("input.txt");

  b = new double[p.matrix_dim];
  x = new double[p.matrix_dim];
  x_new = new double[p.matrix_dim];
  zero_matrix_init(x_new, p.matrix_dim, 1);

  if (my_rank == ROOT_PROC) {
    zero_matrix_init(x, p.matrix_dim, 1);
    matrix = new double[p.matrix_dim * p.matrix_dim];
    if (x_point_formula == "-fps") {
      cout << "===== Using Five-Point-Stencil =====" << endl;
      discretized_grid::laplace(matrix, b, p);
    } else if (x_point_formula == "-nps") {
      cout << "===== Using Nine-Point-Stencil =====" << endl;
      discretized_grid::L2_projection(matrix, b, p);
    } else {
      cout << "Select parallel Discretize method!" << endl;
      exit(1);
    }

    // print_matrix(matrix, p.matrix_dim);
    // print_vector(b, p.matrix_dim);
  }

  double start = MPI_Wtime();
  if (solver == "-dj") {
    if (my_rank == ROOT_PROC) {
      cout << "===== Executing Damped Jacobi =====" << endl;
    }
    p.iterations = damped_jacobi::parallel::run(matrix, x, x_new, b, p);
  } else if (solver == "-gs") {
    if (my_rank == ROOT_PROC) {
      cout << "===== Executing Gauss-Seidel  =====" << endl;
    }
    p.iterations = gauss_seidel::parallel::run(matrix, x, x_new, b, p);
  } else {
    if (my_rank == ROOT_PROC) {
      cout << "Select Method for parallel computation!" << endl;
      exit(1);
    }
  }
  double end = MPI_Wtime();

  double elapsed = end - start;
  p.time = elapsed;

  if (ROOT_PROC == my_rank) {
    cout << endl << "Solution for x is: " << endl;
    print_vector(x, p.matrix_dim);

    cout << endl << "b is: " << endl;
    print_vector(b, p.matrix_dim);

    // cout << endl << "b_check is: " << endl;
    // b_check = new double[p.matrix_dim];
    // matrix_multiplication(b_check, matrix, x, p.matrix_dim, p.matrix_dim, 1);
    // print_vector(b_check, p.matrix_dim);

    cout << "The process took " << elapsed << " seconds to run." << endl;
    p.write_append_csv("output.csv");
  }

  finish_MPI();
}

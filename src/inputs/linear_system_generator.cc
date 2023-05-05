#include "../../include/inputs/linear_system_generator.h"
#include "../../include/helpers/util.h"
#include "../../include/inputs/param.h"

namespace discretized_grid {
double phi_least_square(double x, double y) {
  return x * (1 - x) * y * (1 - y);
};

double f_least_square(double x, double y) {
  return 2 * (y * (1 - y) + x * (1 - x));
};

double phi_nine_point(double x, double y) {
  return (1 - x) * (1 - x) * (1 - y) * (1 - y);
}

double f_nine_point(double x, double y) {
  return 2 * ((x - 1) * (x - 1) + (y - 1) * (y - 1));
}

void five_point_stencil(double *A, double *b, param p) {
  zero_matrix_init(A, p.matrix_dim, p.matrix_dim);
  zero_matrix_init(b, p.matrix_dim, 1);

  int k = 0;
  double h_squared = 1 / (p.step_size_h * p.step_size_h);
  for (int j = 1; j < p.grid_dim; j++) {
    for (int i = 1; i < p.grid_dim; i++) {

      b[k] = f_least_square(i * p.step_size_h, j * p.step_size_h);
      A[k * p.matrix_dim + k] = 4 * h_squared;

      if (i > 1) {
        // cout << "I > 1: " << k - 1 << ", " << k << endl;
        A[(k - 1) * p.matrix_dim + k] = -h_squared;
      } else {
        b[k] = b[k] + h_squared * phi_least_square(0, j * p.step_size_h);
      }
      if (i < p.grid_dim - 1) {
        // cout << "I < N-1: " << k + 1 << ", " << k << endl;
        A[(k + 1) * p.matrix_dim + k] = -h_squared;
      } else {
        b[k] = b[k] + h_squared * phi_least_square(1, j * p.step_size_h);
      }
      if (j > 1) {
        // cout << "J > 1: " << k << ",  " << k - p.grid_dim + 1 << endl;
        A[(k)*p.matrix_dim + k - (p.grid_dim - 1)] = -h_squared;
      } else {
        b[k] = b[k] + h_squared * phi_least_square(i * p.step_size_h, 0);
      }
      if (j < p.grid_dim - 1) {
        // cout << "J < N-1: " << k << ",  " << k - p.grid_dim - 1 << endl;
        A[(k)*p.matrix_dim + k + (p.grid_dim - 1)] = -h_squared;
      } else {
        b[k] = b[k] + h_squared * phi_least_square(i * p.step_size_h, 1);
      }
      k++;
    }
  }
}

void nine_point_stencil(double *A, double *b, param p) {
  zero_matrix_init(A, p.matrix_dim, p.matrix_dim);
  zero_matrix_init(b, p.matrix_dim, 1);

  double T_lower_upper_diagonal = 1.0 / 9;
  double T_diagonal = 4.0 / 9;

  double M_lower_upper_diagonal = 1.0 / 36;
  double M_diagonal = T_lower_upper_diagonal;

  int nr_block_matrices_rows = p.matrix_dim / (p.grid_dim - 1);
  for (int i = 0; i < p.matrix_dim; i += nr_block_matrices_rows) {

    // diagonal
    for (int j = 0; j < nr_block_matrices_rows; j++) {
      int k = i + j;
      A[k * p.matrix_dim + k] = T_diagonal;
      if (j < nr_block_matrices_rows - 1)
        A[k * p.matrix_dim + k + 1] = T_lower_upper_diagonal;
      if (j > 0)
        A[k * p.matrix_dim + k - 1] = T_lower_upper_diagonal;
    }

    // lower diagonal
    if (i > 0) {
      for (int j = 0; j < nr_block_matrices_rows; j++) {
        int l = i + j - nr_block_matrices_rows;
        int k = i + j;
        A[k * p.matrix_dim + l] = M_diagonal;
        if (j < nr_block_matrices_rows - 1)
          A[k * p.matrix_dim + l + 1] = M_lower_upper_diagonal;
        if (j > 0)
          A[k * p.matrix_dim + l - 1] = M_lower_upper_diagonal;
      }
    }

    // upper diagonal
    if (i < p.matrix_dim - nr_block_matrices_rows) {
      for (int j = 0; j < nr_block_matrices_rows; j++) {
        int u = i + j + nr_block_matrices_rows;
        int k = i + j;
        A[k * p.matrix_dim + u] = M_diagonal;
        if (j < nr_block_matrices_rows - 1)
          A[k * p.matrix_dim + u + 1] = M_lower_upper_diagonal;
        if (j > 0)
          A[k * p.matrix_dim + u - 1] = M_lower_upper_diagonal;
      }
    }
  }

  int k = 0;
  double h_squared = p.step_size_h * p.step_size_h;
  for (int j = 1; j < p.grid_dim; j++) {
    for (int i = 1; i < p.grid_dim; i++) {
      b[k] = h_squared * f_nine_point(i * p.step_size_h, j * p.step_size_h);
      k++;
    }
  }
}

} // namespace discretized_grid

// int main(int argc, char *argv[]) {
//   param p(0, 1);
//   p.read_param_from_file("input.txt");
//   cout << p.step_size_h << p.grid_dim << endl;
//
//   double *A = new double[p.matrix_dim * p.matrix_dim];
//   double *b = new double[p.matrix_dim];
//   double *x = new double[p.matrix_dim];
//
//   discretized_grid_linear_system(A, b, p);
//   cout << "Matrix A: " << endl;
//   print_matrix(A, p.matrix_dim);
//   cout << "Solution b: " << endl;
//   print_vector(b, p.matrix_dim);
//
//   return 0;
// }

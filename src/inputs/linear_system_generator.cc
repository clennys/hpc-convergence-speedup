#include "../../include/inputs/linear_system_generator.h"
#include "../../include/helpers/util.h"
#include "../../include/inputs/param.h"

namespace discretized_grid {
double phi_least_square(double x, double y) {
  return x * (1 - x) * y * (1 - y);
};
// double phi_least_square_zero(double x, double y) { return 0.0; };

double f_least_square(double x, double y) {
  return 2 * (y * (1 - y) + x * (1 - x));
};

void five_point_stencil(double *A, double *b, param p) {
  zero_matrix_init(A, p.matrix_dim, p.matrix_dim);

  int k = 0;
  double h_squared = p.step_size_h * p.step_size_h;
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
      k += 1;
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

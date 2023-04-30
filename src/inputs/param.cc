#include "../../include/inputs/param.h"
#include <cassert>
#include <fstream>
#include <iostream>

using namespace std;

param::param(int my_rank, int size, int grid_dim)
    : my_rank(my_rank), size(size), grid_dim(grid_dim) {}

void param::read_param_from_file(string file) {
  double om, eps;
  ifstream ifs(file);
  ifs >> om;
  ifs >> eps;

  matrix_dim = (grid_dim - 1) * (grid_dim - 1);
  omega = om;
  block_length = matrix_dim / size;
  epsilon = eps;
  step_size_h = 1.0 / grid_dim;
  empty_rows = 0;

  if (matrix_dim % size > 0) {
    block_length++;
    empty_rows = block_length - matrix_dim % block_length;
  }

  rank_block_length =
      (my_rank == size - 1) ? block_length - empty_rows : block_length;
  matrix_dim_no_empty_rows = matrix_dim;
  matrix_dim += empty_rows;
}

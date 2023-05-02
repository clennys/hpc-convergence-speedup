#include "../../include/inputs/param.h"
#include <cassert>
#include <fstream>
#include <iostream>

using namespace std;

param::param(int my_rank, int size, int grid_dim, string solver, string x_point)
    : my_rank(my_rank), size(size), grid_dim(grid_dim) {
  this->solver = solver == "-gs" ? "gauss-seidel" : "damped-jacobi";
  this->grid_type = x_point == "-fps" ? "five-point" : "nine-point";
  this->iterations = 0;
}

void param::read(string file) {
  double om, eps;
  ifstream ifs(file);
  ifs >> om;
  ifs >> eps;

  matrix_dim_no_empty_rows = (grid_dim - 1) * (grid_dim - 1);
  omega = om;
  block_length = matrix_dim_no_empty_rows / size;
  epsilon = eps;
  step_size_h = 1.0 / grid_dim;
  // empty_rows = 0;

  rank_block_length = block_length;
  if (matrix_dim_no_empty_rows % size > 0) {
    block_length++;
    // empty_rows = size * block_length - matrix_dim_no_empty_rows;

    int pos = block_length * (my_rank + 1);

    if (pos > matrix_dim_no_empty_rows) {
      rank_block_length =
          max(0, block_length - (pos - matrix_dim_no_empty_rows));
    } else {
      rank_block_length = block_length;
    }
  }

  // matrix_dim_no_empty_rows = matrix_dim;
  // rank_block_length =
  //     (my_rank == size - 1) ? block_length - empty_rows : block_length;
  matrix_dim = size * block_length;

  // cout << "I'm process " << my_rank << " with block size " <<
  // rank_block_length
  // << " matrix_dim " << matrix_dim << endl;
}

void param::write_append_csv(string file) {
  ofstream of;

  of.open(file, ios::out | ios::app);
  string seq = size > 1 ? "parallel" : "serial";
  of << seq << "," << size << "," << grid_dim << "," << matrix_dim_no_empty_rows
     << "," << solver << "," << grid_type << "," << time << "," << iterations
     << "," << epsilon << "," << omega << "," << step_size_h << endl;
}

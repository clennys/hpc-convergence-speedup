#include "../../include/inputs/param.h"
#include <cassert>
#include <fstream>
#include <iostream>

using namespace std;

param::param(int my_rank, int size) : my_rank(my_rank), size(size) {}

void param::read_param_from_file(string file) {
  double omega, epsilon;
  int grid_dim;
  ifstream ifs(file);
  ifs >> omega;
  ifs >> grid_dim;
  ifs >> epsilon;

  this->omega = omega;
  this->grid_dim = grid_dim;
  this->matrix_dim = (grid_dim - 1) * (grid_dim - 1);
  this->block_length = matrix_dim / this->size;
  this->epsilon = epsilon;
  this->step_size_h = 1.0 / grid_dim;

  assert((void("Matrix not split up evenly -> MPIScatter,MPIGather fails"),
          matrix_dim % this->size == 0));
}

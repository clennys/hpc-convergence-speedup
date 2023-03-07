#include "../include/param.h"
#include <cassert>
#include <fstream>
#include <iostream>

using namespace std;

param::param(int my_rank, int size) : my_rank(my_rank), size(size) {}

void param::read_param_from_file(string file) {
  double omega, epsilon;
  int mat_dim;
  ifstream ifs(file);
  ifs >> omega;
  ifs >> mat_dim;
  ifs >> epsilon;

  assert((void("Matrix not split up evenly -> MPIScatter,MPIGather fails"),
          mat_dim % this->size == 0));

  this->omega = omega;
  this->mat_dim = mat_dim;
  this->block_length = mat_dim / this->size;
  this->epsilon = epsilon;
}

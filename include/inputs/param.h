#ifndef PARAM_H_
#define PARAM_H_

#include <string>
using namespace std;

struct param {
  int grid_dim;       // dimension of grid
  int block_length;   // size of submatrix
  int my_rank;        // rank of process
  int size;           // nunber of processes
  double omega;       // damping parameter
  double epsilon;     // error interval
  double step_size_h; // h in notes
  int matrix_dim;     // dimension of matrix

  param(int my_rank, int size);

  void read_param_from_file(string filename);
};

#endif // !PARAM_H_

#ifndef PARAM_H_
#define PARAM_H_

#include <string>
using namespace std;

struct param {
  int grid_dim;          // dimension of grid
  int block_length;      // size of submatrix
  int rank_block_length; // size of submatrix
  int my_rank;           // rank of process
  int size;              // nunber of processes
  double omega;          // damping parameter
  double epsilon;        // error interval
  double step_size_h;    // h in notes
  double time;
  int matrix_dim;               // dimension of matrix
  int matrix_dim_no_empty_rows; // dimension of matrix
  int empty_rows;               // offset if rest
  int iterations;               // iterations

  string solver;
  string grid_type;

  param(int my_rank, int size, int grid_size, string solver, string x_point);

  void read(string filename);
  void write_append_csv(string filename);
};

#endif // !PARAM_H_

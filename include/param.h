#ifndef PARAM_H_
#define PARAM_H_

#include <string>
using namespace std;

struct param {
  int mat_dim;
  int block_length;
  int my_rank;
  int size;
  double omega;
  double epsilon;

  param(int my_rank, int size);

  void read_param_from_file(string filename);
};

#endif // !PARAM_H_

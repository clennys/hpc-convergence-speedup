#include "../include/mpi_module.h"
#include "mpi.h"
#include <iostream>

using namespace std;

int startup_MPI(int *my_rank, int *size) {
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, size);
  MPI_Comm_rank(MPI_COMM_WORLD, my_rank);
  if (*my_rank == ROOT_PROC) {
    cout << "===== Setting up MPI Environment =====" << endl << endl;
  }
  return 0;
}

int create_block_length_for_each_process(int mat_dim, int my_rank, int size) {
  int block_length = mat_dim / size;
  int rest = mat_dim % size;

  if (my_rank < rest) {
    block_length++;
  }

  return block_length;
}

int finish_MPI() {

  MPI_Finalize();

  return 0;
}

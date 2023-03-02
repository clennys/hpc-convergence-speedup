#include "../include/mpi_module.h"
#include "mpi.h"
#include <iostream>

using namespace std;

int startup_MPI(int *my_rank, int *size) {
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, size);
  MPI_Comm_rank(MPI_COMM_WORLD, my_rank);
  if (*my_rank == ROOT_PROC) {
    cout << "===== Setting up MPI Environment =====" << endl;
  }
  return 0;
}

int finish_MPI() {

  MPI_Finalize();

  return 0;
}

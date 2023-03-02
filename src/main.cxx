#include "../include/spd_matrix_generator.h"
#include "../include/util.h"
#include "mpi.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#define N 10
#define COUNT 10
#define NPP 2
#define ROOT_PROC 0

using namespace std;

template <typename T>
bool threshold(T *matrix, T* x_old, T* b, int dim){
	matrix_multiplication(matrix, x_old, dim);

}

int main(int argc, char *argv[]) {
  int my_rank, nr_proc;
  double omega;
  double *b;
  double *mat;
  MPI_Status status;
  int block_length = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nr_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  block_length = N / nr_proc;

  double x_old[N] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double x_new[NPP] = {0, 0};

  b = new double[N];

  if (my_rank == ROOT_PROC) {
    omega = 1;
    b = new double[N]{34917, 45797, 58677,  59040,  74866,
                      81937, 95400, 124676, 119564, 18196};
  }

  // Broadcast omega from Root to all other processes
  MPI_Bcast(&omega, 1, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
  MPI_Bcast(b, N, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);

  if (my_rank == ROOT_PROC) {
    mat = new double[N * N];
    spd_generator(mat, N);
    print_matrix(mat, N);
    cout << endl;
  }

  double *vector = new double[NPP * N];
  MPI_Scatter(mat, NPP * N, MPI_DOUBLE, vector, NPP * N, MPI_DOUBLE, ROOT_PROC,
              MPI_COMM_WORLD);
  int count = 0;
  while (count < COUNT) {
    double sum;
    for (int i = 0; i < NPP; i++) {
      sum = 0;
      for (int j = 0; j < N; j++) {
        sum += vector[i * N + j] * x_old[j];
      }

      x_new[i] = x_old[my_rank * NPP + i] -
                 (omega / vector[i * N + (my_rank * NPP + i)]) *
                     (sum - b[my_rank * NPP + i]);
    }

    MPI_Allgather(x_new, NPP, MPI_DOUBLE, x_old, NPP, MPI_DOUBLE,
                  MPI_COMM_WORLD);

    cout << count << ": I'm " << my_rank << " With gathered x_old: ";
    print_vector(x_old, N);
    cout << count << ": I'm " << my_rank << " With sent x_new: ";
    print_vector(x_new, NPP);

    count++;
  }

  MPI_Finalize();
  return 0;
}

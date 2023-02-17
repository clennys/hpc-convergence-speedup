#include "mpi.h"
#include <iostream>

#define ROOT_PROC 0

using namespace std;

void print_vector(double *vector, int n) {
  for (int i = 0; i < n; i++) {
    cout << *vector << " ";
    vector++;
    cout << endl;
  }
}

void print_matrix(double *matrix, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      cout << matrix[i * n + j] << "\t";
    }
    cout << endl;
  }
}

int main(int argc, char *argv[]) {
  int my_rank, nr_proc;
  double omega;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nr_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  double x[10];

  if (my_rank == ROOT_PROC) {
    omega = 2.0 / 3.0;
    double b[10] = {8, 9, 5, 1, 8, 5, 1, 0, 2, 6};
    double x_init[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  }

  // Broadcast omega from Root to all other processes
  MPI_Bcast(&omega, 1, MPI_DOUBLE, ROOT_PROC, MPI_COMM_WORLD);
  // cout << "I'm " << my_rank << " and I received " << omega << endl;

  if (my_rank == ROOT_PROC) {
    double A[10][10] = {9, 4, 0, 4, 6, 6, 4, 2, 0, 2, 5, 1, 5, 0, 2, 0, 4,
                        4, 7, 4, 8, 1, 7, 1, 7, 4, 4, 1, 3, 8, 9, 3, 0, 2,
                        0, 4, 3, 6, 5, 2, 1, 8, 0, 2, 3, 8, 0, 5, 7, 5, 1,
                        2, 8, 6, 2, 7, 2, 9, 1, 7, 2, 1, 7, 5, 9, 8, 6, 4,
                        9, 1, 8, 7, 0, 2, 9, 9, 4, 0, 2, 3, 4, 5, 6, 6, 0,
                        4, 0, 8, 5, 6, 2, 6, 5, 6, 3, 5, 7, 1, 0, 6};
    for (int proc = 1; proc < nr_proc; proc++) {
      MPI_Send(&A[(proc)*2], 20, MPI_DOUBLE, proc, 100, MPI_COMM_WORLD);
    }

    cout << "I'm " << my_rank << " and I received: " << endl;
    print_matrix(&A[0][0], 2, 10);
  } else {
    double vector[2][10];
    MPI_Recv(&vector[0][0], 20, MPI_DOUBLE, ROOT_PROC, 100, MPI_COMM_WORLD,
             &status);
    cout << "I'm " << my_rank << " and I received: " << endl;
    print_matrix(&vector[0][0], 2, 10);
    cout << endl;
  }

  MPI_Finalize();
  return 0;
}

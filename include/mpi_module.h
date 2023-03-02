#ifndef MPI_MODULE_H_
#define MPI_MODULE_H_

#define ROOT_PROC 0
int startup_MPI(int *my_rank, int *size);
int finish_MPI();

#endif // !MPI_MODULE
